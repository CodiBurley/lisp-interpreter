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
#include <set>
#include <iostream>
#include <algorithm>
#include "SExp.h"

using std::string;
using std::vector;


const std::set<string> WHITE_SPACE_CHARS = { " ", "\n", "\t" };
bool isWhite(string s) { return WHITE_SPACE_CHARS.count(s) != 0; }

enum TokenType { LParen, RParen, Dot, White, ID, Num, Invalid };
struct Token {
  Token(TokenType t = Invalid, string v = ""): type(t), value(v) {}

  TokenType type;
  string value;
};

// PROTOTYPES
SExp* input(string s);
SExp* input(vector<Token> tokens);
SExp* inputList(vector<Token> tokens);


void printTokens(vector<Token> tokens) {
  for (Token t: tokens) {
    std::cout << "'"<< t.value << "' ";
  }
  std::cout << std::endl;
}

vector<string> tokenizeToStrs(const string& strInput, const string& strDelims) {
 vector<string> vS;

 string strOne = strInput;
 string delimiters = strDelims;

 size_t startpos = 0;
 size_t pos = strOne.find_first_of(delimiters, startpos);

 while (string::npos != pos && string::npos != startpos)
 {
  if(strOne.substr(startpos, pos - startpos) != "" && !isWhite(strOne.substr(startpos, pos - startpos)))
   vS.push_back(strOne.substr(startpos, pos - startpos));

  if (!isWhite(strOne.substr(pos, 1))) {
   vS.push_back(strOne.substr(pos, 1));
  }

   startpos = pos + 1;

   pos = strOne.find_first_of(delimiters, startpos);
 }

 if (!vS.size()) vS.push_back(strInput);

 return vS;
}

bool isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    if (s.size() > 1 && (s.at(0) == '+' || s.at(0) == '-')) {
      ++it;
    }
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool validID(string str)
{
    return str.size() > 0 &&
      !isNumber(str.substr(0,1)) &&
      find_if(str.begin(), str.end(), [](char c) { return !(isalnum(c) || (c == ' ')); }) == str.end();
}

vector<Token> tokenStringsToTokenEnums(vector<string> strs) {
  vector<Token> enums(strs.size());
  std::transform(strs.begin(), strs.end(), enums.begin(), [](string s) {
    if (s == "(") return Token(LParen, s);
    else if (s == ")") return Token(RParen, s);
    else if (s == ".") return Token(Dot, s);
    else if (s == " " || s == "\t" || s == "\r" || s == "\n") return Token(White, s);
    else if (isNumber(s)) return Token(Num, s);
    else if (validID(s)) return Token(ID, s);
    else return Token(Invalid, s);
  });
  return enums;
}

vector<Token> tokenize(string s) {
  return tokenStringsToTokenEnums(tokenizeToStrs(s, "( \n\t\r.)"));
}

size_t nextNonWhite(vector<Token> ts, size_t from) {
  if (ts.size() <= from) return ts.size();
  vector<Token>::iterator it = std::find_if(
      ts.begin() + from,
      ts.end(),
      [](Token t) { return t.type != White; }
  );
  return (it == ts.end()) ? ts.size() : std::distance(ts.begin(), it);
}

size_t nextTokenOutsideParens(vector<Token> ts, size_t from, size_t startingOpenParens = 0) {
  if (ts.size() <= from) return ts.size();

  if (startingOpenParens == 0 && ts.at(from).type != LParen) return from;

  int openParens = startingOpenParens;
  size_t currentTokenIdx = from;

  do {
    Token currentToken = ts.at(currentTokenIdx);
    if (currentToken.type == LParen) openParens++;
    else if (currentToken.type == RParen) openParens--;
    currentTokenIdx++;
  } while (openParens > 0 && currentTokenIdx < ts.size());

  return currentTokenIdx;
}

size_t nextClosingParen(vector<Token> ts, size_t from) {
  if (ts.size() <= from) return ts.size();

  vector<Token>::iterator it = std::find_if(
      ts.begin() + from,
      ts.end(),
      [](Token t) { return t.type == RParen; }
  );
  return (it == ts.end()) ? ts.size() : std::distance(ts.begin(), it);
}


SExp* input(vector<Token> tokens) {
  if (!tokens.size()) return new SExp(Error("no input given"));
  Token firstToken = tokens.at(0);

  switch(firstToken.type) {
    case ID: {
      //std::cout << "creating symbol" << std::endl;
      if (tokens.size() > 1) return new SExp(Error("unexpected " + tokens.at(1).value));
      return findOrCreateSymbolic(firstToken.value);
    }
    case Num: {
      return new SExp(std::stoi(firstToken.value));
    }
    case LParen: {
      size_t nextTokenIdx = nextNonWhite(tokens, 1);
      if (nextTokenIdx == tokens.size()) return new SExp(Error("missing closing paren")); // error

      Token nextToken = tokens.at(nextTokenIdx);
      if (nextToken.type == Invalid || nextToken.type == Dot) return new SExp(Error("unexpected " + nextToken.value)); // error
      else if (nextToken.type == RParen) return findOrCreateSymbolic("NIL"); // acually nil

      size_t startingOpenParens = (nextToken.type == LParen) ? 1 : 0;
      size_t nextTokenOutParensIdx = nextTokenOutsideParens(tokens, nextTokenIdx + 1, startingOpenParens);
      if (nextTokenOutParensIdx == tokens.size()) return new SExp(Error("missing closing paren")); // error

      Token nextTokenOutParens = tokens.at(nextTokenOutParensIdx);
      if (nextTokenOutParens.type == Dot) {
        size_t nextRightTokenOutParensIdx = nextTokenOutsideParens(tokens,
            nextTokenOutParensIdx + 1);
        size_t nextClosingParenIdx = nextClosingParen(tokens, nextRightTokenOutParensIdx);
        vector<Token> leftTokens(tokens.begin() + nextTokenIdx,
            tokens.begin() + nextTokenOutParensIdx);
        vector<Token> rightTokens(tokens.begin() + nextTokenOutParensIdx + 1, --tokens.end());
        //std::cout << "FOR TOKENS: ";
        //printTokens(tokens);
        //std::cout << "-- LEFT --" << std::endl;
        //printTokens(leftTokens);
        //std::cout << "-- RIGHT --" << std::endl;
        //printTokens(rightTokens);
        //std::cout << "-- END LEFT RIGHT --" << std::endl;
        return cons(input(leftTokens), input(rightTokens));
      } else {
        vector<Token> tokensLessOpenParen(++tokens.begin(), tokens.end());
        return inputList(tokensLessOpenParen);
      }

      return new SExp(Error("internal error"));
    }
    default: {
      return new SExp(Error("SExp can only start with (, number, or id")); // error
    }
  }
}

SExp* inputList(vector<Token> tokens) {
  Token firstToken = tokens.at(0);

  switch(firstToken.type) {
    case RParen: {
      if (tokens.size() > 1) return new SExp(Error("unexpected " + tokens.at(1).value));
      return findOrCreateSymbolic("NIL");
    }
    case ID: {
      vector<Token> rest(++tokens.begin(), tokens.end());
      return cons(findOrCreateSymbolic(firstToken.value), inputList(rest));
    }
    case Num: {
      vector<Token> rest(++tokens.begin(), tokens.end());
      return cons(new SExp(std::stoi(firstToken.value)), inputList(rest));
    }
    case LParen: {
      size_t closingParenIdx = nextTokenOutsideParens(tokens, 0) - 1;
      //std::cout << closingParenIdx << std::endl;
      if (closingParenIdx == tokens.size()) return new SExp(Error("missing closing paren"));

      vector<Token> nextItemTokens(tokens.begin(), tokens.begin() + closingParenIdx + 1);
      vector<Token> rest(tokens.begin() + closingParenIdx + 1, tokens.end());
      //std::cout << "Item: " << std::endl;
      //printTokens(nextItemTokens);
      //std::cout << "Rest: " << std::endl;
      //printTokens(rest);

      return cons(input(nextItemTokens), inputList(rest));
    }
    default: {
      return new SExp(Error("unexpected token '" + firstToken.value + "' in list"));
    }
  }
}

SExp* input(string s) {
  vector<Token> tokens = tokenize(s);
  return input(tokens);
}
