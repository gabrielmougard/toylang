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

#include "global/global.h"

// This is an object that owns LLVM core data structures
llvm::LLVMContext TheContext;

// This is a helper object that makes easy to generate LLVM instructions
llvm::IRBuilder<> Builder(TheContext);

// This is an LLVM construct that contains functions and global variables
std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
std::map<std::string, llvm::Value *> NamedValues;