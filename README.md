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

![](./toylang.png)

A toy language based on LLVM.

## Examples

```
ready> extern sin(x);
ready> Read extern: 
declare double @sin(double)

ready> extern cos(x);
ready> Read extern: 
declare double @cos(double)

ready> sin(1.0);
ready> Read top-level expression: 
define double @__anon_expr() {
entry:
  %calltmp = call double @sin(double 1.000000e+00)
  ret double %calltmp
}

Evaluated to 0.841471
ready> def foo(x) sin(x)*sin(x) + cos(x)*cos(x);
ready> Read function definition: 
define double @foo(double %x) {
entry:
  %calltmp = call double @sin(double %x)
  %calltmp1 = call double @sin(double %x)
  %multmp = fmul double %calltmp, %calltmp1
  %calltmp2 = call double @cos(double %x)
  %calltmp3 = call double @cos(double %x)
  %multmp4 = fmul double %calltmp2, %calltmp3
  %addtmp = fadd double %multmp, %multmp4
  ret double %addtmp
}

ready> foo(4.0);
ready> Read top-level expression: 
define double @__anon_expr() {
entry:
  %calltmp = call double @foo(double 4.000000e+00)
  ret double %calltmp
}

Evaluated to 1.000000
ready> def testfunc(x y) x + y*2;
ready> Read function definition: 
define double @testfunc(double %x, double %y) {
entry:
  %multmp = fmul double %y, 2.000000e+00
  %addtmp = fadd double %x, %multmp
  ret double %addtmp
}

ready> testfunc(4, 10);
ready> Read top-level expression: 
define double @__anon_expr() {
entry:
  %calltmp = call double @testfunc(double 4.000000e+00, double 1.000000e+01)
  ret double %calltmp
}

Evaluated to 24.000000
ready> testfunc(5, 10);
ready> Read top-level expression: 
define double @__anon_expr() {
entry:
  %calltmp = call double @testfunc(double 5.000000e+00, double 1.000000e+01)
  ret double %calltmp
}

Evaluated to 25.000000

ready> extern foo();
ready> Read extern: 
declare double @foo()

ready> extern bar();
ready> Read extern: 
declare double @bar()

ready> def baz(x) if x then foo() else bar();
ready> Read function definition: 
define double @baz(double %x) {
entry:
  %ifcond = fcmp one double %x, 0.000000e+00
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  %calltmp = call double @foo()
  br label %ifcont

else:                                             ; preds = %entry
  %calltmp1 = call double @bar()
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %iftmp = phi double [ %calltmp, %then ], [ %calltmp1, %else ]
  ret double %iftmp
}
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

* JIT compilation + optimizer support
* Control flow logic + for-loop
* [TODO] User-defined operator
