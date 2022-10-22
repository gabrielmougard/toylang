// Copyright 2022 gab
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ast/ForExprAST.h"

// Generates LLVM code for conditional block declarations
llvm::Value *ForExprAST::codegen() {
    // Emit the start code first, without 'variable' in scope.
    llvm::Value *StartVal = Start->codegen();
    if (!StartVal)
        return nullptr;

    // Make the new basic block for the loop header, inserting after current
    // block.
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *PreheaderBB = Builder.GetInsertBlock();
    llvm::BasicBlock *LoopBB =
        llvm::BasicBlock::Create(TheContext, "loop", TheFunction);

    // Insert an explicit fall through from the current block to the LoopBB.
    Builder.CreateBr(LoopBB);

    // Start insertion in LoopBB.
    Builder.SetInsertPoint(LoopBB);

    // Start the PHI node with an entry for Start.
    llvm::PHINode *Variable = Builder.CreatePHI(llvm::Type::getDoubleTy(TheContext),
                                        2, VarName.c_str());
    Variable->addIncoming(StartVal, PreheaderBB);

    // Within the loop, the variable is defined equal to the PHI node.  If it
    // shadows an existing variable, we have to restore it, so save it now.
    llvm::Value *OldVal = NamedValues[VarName];
    NamedValues[VarName] = Variable;

    // Emit the body of the loop.  This, like any other expr, can change the
    // current BB.  Note that we ignore the value computed by the body, but don't
    // allow an error.
    if (!Body->codegen())
        return nullptr;

    // Emit the step value.
    llvm::Value *StepVal = nullptr;
    if (Step) {
        StepVal = Step->codegen();
        if (!StepVal)
            return nullptr;
    } else {
        // If not specified, use 1.0.
        StepVal = llvm::ConstantFP::get(TheContext, llvm::APFloat(1.0));
    }

    llvm::Value *NextVar = Builder.CreateFAdd(Variable, StepVal, "nextvar");

    // Compute the end condition.
    llvm::Value *EndCond = End->codegen();
    if (!EndCond)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.0.
    EndCond = Builder.CreateFCmpONE(
        EndCond, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "loopcond");

    // Create the "after loop" block and insert it.
    llvm::BasicBlock *LoopEndBB = Builder.GetInsertBlock();
    llvm::BasicBlock *AfterBB =
        llvm::BasicBlock::Create(TheContext, "afterloop", TheFunction);

    // Insert the conditional branch into the end of LoopEndBB.
    Builder.CreateCondBr(EndCond, LoopBB, AfterBB);

    // Any new code will be inserted in AfterBB.
    Builder.SetInsertPoint(AfterBB);

    // Add a new entry to the PHI node for the backedge.
    Variable->addIncoming(NextVar, LoopEndBB);

    // Restore the unshadowed variable.
    if (OldVal)
        NamedValues[VarName] = OldVal;
    else
        NamedValues.erase(VarName);

    // for expr always returns 0.0.
    return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(TheContext));
}