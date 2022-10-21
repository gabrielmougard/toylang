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


#ifndef __LEXER_H__
#define __LEXER_H__

#include <cstdlib>
#include <string>

// Provide a simple token buffer
// CurTok is the current token the parser is looking at
// getNextToken reads another token from the lexer and updates CurTok with its results
extern int CurTok;
int gettok();
int getNextToken();

// If the current token is an identifier
// IdentifierStr will hold the name of the identifier
extern std::string IdentifierStr;

// If the current token is a numeric literal
// NumVal holds its value
extern double NumVal;

#endif