#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

enum SExpType { Int, Symbol, NonAtom };

class SExp {
public:
  SExpType type; /* 1: integer atom; 2: symbolic atom; 3: non-atom */
  int val; /* if type is 1 */
  string name; /* if type is 2 */
  SExp* left; SExp* right; /* if type is 3 */

  static vector<SExp*> idPointers;

  SExp(int v): type(Int), val(v) {}
  SExp(string n): type(Symbol), name(n) {}
  SExp(SExp* l, SExp* r): type(NonAtom), left(l), right(r) {}
};

vector<SExp*> SExp::idPointers = vector<SExp*>();

static SExp* findOrCreateSymbolic(string id) {
  vector<SExp*>::iterator it = std::find_if(SExp::idPointers.begin(), SExp::idPointers.end(), [id](SExp* se) {
    if (se->type != Symbol) return false;
    return se->name == id;
  });

  if (it == SExp::idPointers.end()) {
    SExp* newSym = new SExp(id);
    SExp::idPointers.push_back(newSym);
    return newSym;
  }

  return *it;
}

static SExp* cons(SExp* l, SExp* r) { return new SExp(l, r); }
