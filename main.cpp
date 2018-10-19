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
  input(testStr);
  return 0;
}
