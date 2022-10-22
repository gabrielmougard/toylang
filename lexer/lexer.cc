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

#include "lexer/lexer.h"
#include "lexer/token.h"

int CurTok;
std::string IdentifierStr;
double NumVal;

// The actual implementation of the lexer is a single function gettok()
// It's called to return the next token from standard input
// gettok works by calling getchar() function to read chars one at a time
// Then it recognizes them and stores the last character read in LastChar
int gettok() {
  static int LastChar = ' ';

  // The first thing we need to do is ignore whitespaces between tokens
  while (isspace(LastChar)) {
    LastChar = getchar();
  }

  // Next thing is recognize identifier and specific keywords like "def"
  if (isalpha(LastChar)) {
    IdentifierStr = LastChar;

    // Stacking together all alphanumeric characters into IdentifierStr
    while (isalnum(LastChar = getchar())) {
      IdentifierStr += LastChar;
    }

    if (IdentifierStr == "def")
      return tok_def;

    if (IdentifierStr == "extern")
      return tok_extern;

    if (IdentifierStr == "if")
      return tok_if;

    if (IdentifierStr == "then")
      return tok_then;

    if (IdentifierStr == "else")
      return tok_else;
    
    if (IdentifierStr == "for")
      return tok_for;

    if (IdentifierStr == "in")
      return tok_in;

    return tok_identifier;
  }

  // Stacking together only numeric values
  if (isdigit(LastChar) || LastChar == '.') {
    std::string NumStr;

    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    // Convert numeric string to numeric value
    // that we are store in NumVal
    NumVal = strtod(NumStr.c_str(), 0);
    return tok_number;
  }

  // Handling comments by skipping to the end of the line
  // and return the next token
  if (LastChar == '#') {
    do {
      LastChar = getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) {
      return gettok();
    }
  }

  // Finally, if the input doesn't match one of the above cases
  // it's either an operator character like '+' or the end of the file
  if (LastChar == EOF) {
    return tok_eof;
  }

  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

int getNextToken() {
  return CurTok = gettok();
}