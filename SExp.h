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
  //static SExp* findOrCreateSymbolic(string id) {
  //  vector<SExp*>::iterator it = std::find_if(idPointers.begin(), idPointers.end(), [id](SExp* se) {
  //    if (se->type != Symbol) return false;
  //    return se->name == id;
  //  });

  //  return (it == idPointers.end()) ? (new SExp(id)) : *it;
  //}

  SExp(int v): val(v) {}
  SExp(string n): name(n) {} 
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
