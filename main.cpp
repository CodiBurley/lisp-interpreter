//#include "SExp.h"
#include "Input.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using std::unique_ptr;
using std::string;
using std::vector;

int main() {
  string testStr = "(func . (1 . hello))";
  string testStr1 = "hello";
  SExp* s = input(testStr);
  std::cout << "SExp symbol: " << std::endl;
  if (s->type == Symbol) {
    std::cout << s->name << std::endl;
  }
  return 0;
}
