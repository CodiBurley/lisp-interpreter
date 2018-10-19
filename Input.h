/*
 * =====================================================================================
 *
 *       Filename:  Input.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/18/2018 21:17:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "SExp.h"

using std::string;
using std::vector;

enum TokenType { LParen, RParen, Dot, White, ID, Num, Invalid };
struct Token {
  Token(TokenType t = Invalid, string v = ""): type(t), value(v) {}

  TokenType type;
  string value;
};

vector<string> tokenizeToStrs(const string& strInput, const string& strDelims)
{
 vector<string> vS;

 string strOne = strInput;
 string delimiters = strDelims;

 int startpos = 0;
 int pos = strOne.find_first_of(delimiters, startpos);

 while (string::npos != pos || string::npos != startpos)
 {
  if(strOne.substr(startpos, pos - startpos) != "")
   vS.push_back(strOne.substr(startpos, pos - startpos));

  // if delimiter is a new line (\n) then addt new line
  if(strOne.substr(pos, 1) == "\n")
   vS.push_back("\\n");
  // else if the delimiter is not a space
  else if (strOne.substr(pos, 1) != " ")
   vS.push_back(strOne.substr(pos, 1));

  if( string::npos == strOne.find_first_not_of(delimiters, pos) )
   startpos = strOne.find_first_not_of(delimiters, pos);
  else
   startpos = pos + 1;

   pos = strOne.find_first_of(delimiters, startpos);

 }

 return vS;
}

bool isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

vector<Token> tokenStringsToTokenEnums(vector<string> strs) {
  vector<Token> enums(strs.size());
  std::transform(strs.begin(), strs.end(), enums.begin(), [](string s) {
    if (s == "(") return Token(LParen);
    else if (s == ")") return Token(RParen);
    else if (s == ".") return Token(Dot);
    else if (s == " " || s == "\t" || s == "\r" || s == "\n") return Token(White);
    else if (isNumber(s)) return Token(Num, s);
    else return Token(ID, s);
  });
  return enums;
}

vector<Token> tokenize(string s) {
  return tokenStringsToTokenEnums(tokenizeToStrs(s, "( .)"));
}


// TODO put input in its own file
void input(string s) {
  vector<Token> tokens = tokenize(s);
  for (Token t : tokens) {
    if (t.type == ID) {
      findOrCreateSymbolic(t.value);
    }
  }
}
