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

// lexer headers
#include "lexer/lexer.h"
#include "lexer/token.h"

// AST headers
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"

// parser headers
#include "parser/parser.h"

// logger headers
#include "logger/logger.h"

// global headers
#include "global/global.h"

// LLVM headers
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

// stdlib headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

static void HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition: \n");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleExtern() {
    if (auto ProtoAST = ParseExtern()) {
        if (auto *FnIR = ProtoAST->codegen()) {
            fprintf(stderr, "Read extern: \n");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (auto FnAST = ParseTopLevelExpr()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read top-level expression: \n");
            FnIR->print(errs());
            fprintf(stderr, "\n");

            // Remove the anonymous expression.
            FnIR->eraseFromParent();
        }
    } else {
        getNextToken();
    }
}

static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");

        switch (CurTok) {
        case tok_eof:
            return;
        case ';':
            getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;

    fprintf(stderr, "ready> ");
    getNextToken();

    TheModule = std::make_unique<Module>("My awesome JIT", TheContext);

    MainLoop();

    TheModule->print(errs(), nullptr);

    return 0;
}