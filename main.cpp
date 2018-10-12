#include "SExp.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using std::unique_ptr;
using std::string;
using std::vector;

vector<string> tokenize(const std::string& str, const std::string& regex)
{
	vector<string> tokens = {std::sregex_token_iterator(str.begin(), str.end(), std::regex(regex), {-1,0}), std::sregex_token_iterator()};
  vector<string> withoutEmpty;

// copy only positive numbers:
  std::copy_if (tokens.begin(), tokens.end(), std::back_inserter(withoutEmpty), [](string s){return s.size() > 0;});
  return withoutEmpty;
}

// TODO put input in its own file
void input(vector<string> tokens) {
  for (string s : tokens) {
    std::cout <<"\"" << s << "\"" << std::endl;
  }
}

int main() {
  string testStr = "(func . (1 . hello))";
  input(tokenize(testStr, "[\\(\\s\\.\\)]"));
  return 0;
}
