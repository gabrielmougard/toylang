<!--
 Copyright 2022 gab
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-->

# Toylang

A toy language based on LLVM.

## Examples

```
// function definition
def foo(a b) a*a + 2*a*b + b*b;

// external function
extern cos(x);

// binary operations
4+5

4*5
```

## Dependencies

```
apt-get install clang-format clang-tidy clang-tools clang clangd libc++-dev libc++1 libc++abi-dev libc++abi1 libclang-dev libclang1 liblldb-dev libllvm-ocaml-dev libomp-dev libomp5 lld lldb llvm-dev llvm-runtime llvm python3-clang
```

## How to build

`make main && ./main`

## Features

* Lexer + Parser
    * variable assignment
    * binary operations
    * function definition
    * external function definition

* [TODO] Control flow logic
* [TODO] User-defined operator
* [TODO] JIT compilation + optimizer support
