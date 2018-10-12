#include<string>

using std::string;

class SExp {
  static std::shared_ptr<SExp> idPointers[20];
};

class NonAtom: SExp {
  std::unique_ptr<SExp> left, right;
};

class IntAtom: SExp {
  int value;
};

class SymbolAtom: SExp {
  string name;
};
