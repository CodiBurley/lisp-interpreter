#ifndef SEXP_H
#define SEXP_H

#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;


enum SExpType { Int, Symbol, NonAtom, Err };

class Error {
public:
  Error(string s = ""): message("**error**: " + s) {}
  string message;
};

class SExp {
public:
  SExpType type; /* 1: integer atom; 2: symbolic atom; 3: non-atom */
  int val; /* if type is 1 */
  string name; /* if type is 2 */
  Error err;
  SExp* left; SExp* right; /* if type is 3 */

  static vector<SExp*> idPointers;

  SExp(int v): type(Int), val(v) {}
  SExp(string n): type(Symbol), name(n) {}
  SExp(Error e): type(Err), err(e) {}
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

// init primitive symbols
static SExp* T = findOrCreateSymbolic("T");
static SExp* NIL = findOrCreateSymbolic("NIL");
static SExp* QUOTE = findOrCreateSymbolic("QUOTE");
static SExp* COND = findOrCreateSymbolic("COND");
static SExp* CAR = findOrCreateSymbolic("CAR");
static SExp* CDR = findOrCreateSymbolic("CDR");
static SExp* CONS = findOrCreateSymbolic("CONS");
static SExp* ATOM = findOrCreateSymbolic("ATOM");
static SExp* NULL_SYM = findOrCreateSymbolic("NULL");
static SExp* EQ = findOrCreateSymbolic("EQ");
static SExp* INT = findOrCreateSymbolic("INT");
static SExp* PLUS = findOrCreateSymbolic("PLUS");
static SExp* MINUS = findOrCreateSymbolic("MINUS");
static SExp* TIMES = findOrCreateSymbolic("TIMES");
static SExp* QUOTIENT = findOrCreateSymbolic("QUOTIENT");
static SExp* REMAINDER = findOrCreateSymbolic("REMAINDER");
static SExp* LESS = findOrCreateSymbolic("LESS");
static SExp* GREATER = findOrCreateSymbolic("GREATER");
static SExp* DEFUN = findOrCreateSymbolic("DEFUN");


static SExp* cons(SExp* l, SExp* r) { return new SExp(l, r); }
static SExp* car(SExp* e) {
  if (e->type != NonAtom) return new SExp(Error("can only call car on a non atomic s expression"));
  return e->left;
}
static SExp* cdr(SExp* e) {
  if (e->type != NonAtom) return new SExp(Error("can only call cdr on a non atomic s expression"));
  return e->right;
}
static SExp* caar(SExp* e) {
  if (e->type != NonAtom) return new SExp(Error("can only call caar on a non atomic s expression"));
  return car(car(e));
}
static SExp* cadr(SExp* e) {
  if (e->type != NonAtom) return new SExp(Error("can only call cadr on a non atomic s expression"));
  return car(cdr(e));
}

static bool atom(SExp* s) { return s->type == Int || s->type == Symbol; }
static bool isInt(SExp* s) { return s->type == Int; }


static bool eq(SExp* s1, SExp* s2) {
  if (s1->type != s2->type) return false;
  if (s1->type == NonAtom || s1->type == Err) return false;
  return (s1->type == Int) ? s1->val == s2->val : s1 == s2;
}

static bool null(SExp* s) { return eq(s, NIL); }

static SExp* add(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only add INTS"));
  return new SExp(s1->val + s2->val);
}

static SExp* subtract(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only subtract INTS"));
  return new SExp(s1->val - s2->val);
}

static SExp* multiply(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only multiply INTS"));
  return new SExp(s1->val * s2->val);
}

static SExp* quotient(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only divide INTS"));
  return new SExp(s1->val / s2->val);
}

static SExp* remainder(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only modulo INTS"));
  return new SExp(s1->val % s2->val);
}

static bool lesst(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only compare inequality less than of INTS"));
  return s1->val < s2->val;
}

static bool greatert(SExp* s1, SExp* s2) {
  if (s1->type != Int || s2->type != Int) return new SExp(Error("can only compare inequality greater than of INTS"));
  return s1->val > s2->val;
}

#endif
