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

#include "ast/CallExprAST.h"

// Generate LLVM code for function calls
llvm::Value *CallExprAST::codegen() {
  llvm::Function *CalleeF = TheModule->getFunction(Callee);

  if (!CalleeF) {
    return LogErrorV("Unknown function referenced");
  }

  if (CalleeF->arg_size() != Args.size()) {
    return LogErrorV("Incorrect # arguments passed");
  }

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; i++) {
    ArgsV.push_back(Args[i]->codegen());

    if (!ArgsV.back()) {
      return nullptr;
    }
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}