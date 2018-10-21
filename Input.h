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

 size_t startpos = 0;
 size_t pos = strOne.find_first_of(delimiters, startpos);

 while (string::npos != pos && string::npos != startpos)
 {
  std::cout << "in while loop" << std::endl;
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

 if (!vS.size()) vS.push_back(strInput);

 std::cout << "TOKENS:" << std::endl;
 for (string s: vS) {
  std::cout << s << std::endl;
 }
 std::cout << "END TOKENS" << std::endl;

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

size_t nextNonWhite(vector<Token> ts, size_t from) {
  if (ts.size() < from) return ts.size();
  vector<Token>::iterator it = std::find_if(
      ts.begin() + from,
      ts.end(),
      [](Token t) { return t.type != White; }
  );
  return (it == ts.end()) ? ts.size() : std::distance(ts.begin(), it);
}

SExp* input(string s) {
  vector<Token> tokens = tokenize(s);
  std::cout << tokens.at(0).value << std::endl;
  if (!tokens.size()) return new SExp("NIL");
  Token firstToken = tokens.at(0);

  switch(firstToken.type) {
    case ID: {
      std::cout << "creating symbol" << std::endl;
      return findOrCreateSymbolic(firstToken.value);
      break;
    }
    case LParen: {
      size_t nextTokenIdx = nextNonWhite(tokens, 1);
      if (nextTokenIdx == tokens.size()) return new SExp("ERR"); // error

      Token nextToken = tokens.at(nextTokenIdx);
      if (nextToken.type == Invalid || nextToken.type == Dot) return new SExp("ERR"); // error
      else if (nextToken.type == RParen) return new SExp("NIL"); // acually nil
      return new SExp("ERR");
      break;
    }
    default: {
      return new SExp("ERR"); // error
      break;
    }
  }
}
