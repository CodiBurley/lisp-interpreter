//#include "SExp.h"
#include "Input.h"
#include "Output.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using std::unique_ptr;
using std::string;
using std::vector;

int main() {
  //string testStr = "((3 . func) . (1 . hello) . 4)";
  //string testStr1 = "(hey. (sister . (brother . mother )))";
  //string testL = "((hey hoe) . 3)";
  //SExp* s = input(testL);
  string expStr;
  while (std::getline(std::cin, expStr)) {
    if (expStr == "$") {
      continue;
    } else if (expStr == "$$") {
      std::cout << "BYE!" << std::endl;
      return 0;
    }
    SExp* s = input(expStr);
    string out = output(s);
    std::cout << "in: " << expStr << std::endl;
    std::cout << "> " << out << std::endl;
  }
  //std::cout << "SExp symbol: " << std::endl;
  //if (s->type == Symbol) {
  //  std::cout << s->name << std::endl;
  //}
  return 0;
}
