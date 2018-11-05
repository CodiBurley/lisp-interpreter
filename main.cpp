//#include "SExp.h"
#include "Input.h"
#include "Eval.h"
#include "Output.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using std::unique_ptr;
using std::string;
using std::vector;

int main() {
  string readStr;
  string expStr = "";
  while (std::getline(std::cin, readStr)) {
    if (readStr == "$") {
      SExp* s = input(expStr);
      SExp* r = eval(s);
      string out = output(r);
      std::cout << "in: " << expStr << std::endl;
      std::cout << "> " << out << std::endl;
      expStr = "";
      continue;
    } else if (readStr == "$$") {
      SExp* s = input(expStr);
      SExp* r = eval(s);
      string out = output(r);
      std::cout << "in: " << expStr << std::endl;
      std::cout << "> " << out << std::endl;
      std::cout << "BYE!" << std::endl;
      return 0;
    }
    //expStr = expStr + " " + readStr;
    expStr = expStr  + readStr;
  }
  return 0;
}
