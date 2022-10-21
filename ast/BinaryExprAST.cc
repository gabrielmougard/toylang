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

#include "ast/BinaryExprAST.h"
#include "global/global.h"

// Generate LLVM code for binary expressions
llvm::Value *BinaryExprAST::codegen() {
  llvm::Value *L = LHS->codegen();
  llvm::Value *R = RHS->codegen();

  if (!L || !R) {
    return nullptr;
  }

  switch (Op) {
    case '+':
      return Builder.CreateFAdd(L, R, "addtmp");
    case '-':
      return Builder.CreateFSub(L, R, "subtmp");
    case '*':
      return Builder.CreateFMul(L, R, "multmp");
    case '<':
      L = Builder.CreateFCmpULT(L, R, "cmptmp");
      return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    default:
      return LogErrorV("Invalid binary operator");
  }
}