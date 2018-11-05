#ifndef EVAL_H
#define EVAL_H

#include "SExp.h"
#include "Output.h"
#include<string>
#include<map>

#include<iostream>

using std::string;
using std::cout;
using std::endl;

typedef std::map<string, SExp*> AList;
typedef AList DList;
typedef std::pair<string, SExp*> AListElem;
typedef std::pair<string, SExp*> DListElem;

static DList dList;

// prototypes
SExp* eval(SExp* exp, AList aList);

bool in(SExp* e, AList aL) {
  if (e->type != Symbol) return false;
  return aL.find(e->name) != aL.end();
}

SExp* getFuncValExp(string funcName) {
  if (dList.find(funcName) == dList.end()) return new SExp(Error("no definition of function: " + funcName));
  return dList.at(funcName);
}

AList addPairs(SExp* namesLExp, SExp* valsLExp, AList aList) {
  if (namesLExp == NIL && valsLExp == NIL) return aList;
  if (namesLExp == NIL || valsLExp == NIL) {
    throw "wrong number of arguments";
  };

  string name = car(namesLExp)->name;
  SExp* val = car(valsLExp);
  aList[name] = val;
  return addPairs(cdr(namesLExp), cdr(valsLExp), aList);
}

SExp* apply(SExp* funcExp, SExp* argsExp, AList aList) {
  //cout << "In apply: " << output(funcExp) << endl;
  //cout << output(argsExp) << endl;
  if (!atom(funcExp)) return new SExp(Error("not a lisp expression"));
  Error err;
  bool isErr = findError(argsExp, err);
  if (isErr) return new SExp(err);

  if (eq(funcExp, CAR)) return caar(argsExp);
  else if (eq(funcExp, CDR)) return cdr(car(argsExp));
  else if (eq(funcExp, CONS)) return cons(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, ATOM)) return atom(car(argsExp)) ? T : NIL;
  else if (eq(funcExp, NULL_SYM)) return null(car(argsExp)) ? T : NIL;
  else if (eq(funcExp, EQ)) return eq(car(argsExp), car(cdr(argsExp))) ? T : NIL;
  else if (eq(funcExp, INT)) return isInt(car(argsExp)) ? T : NIL;
  else if (eq(funcExp, PLUS)) return add(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, MINUS)) return subtract(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, TIMES)) return multiply(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, QUOTIENT)) return quotient(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, REMAINDER)) return remainder(car(argsExp), car(cdr(argsExp)));
  else if (eq(funcExp, LESS)) return lesst(car(argsExp), car(cdr(argsExp))) ? T : NIL;
  else if (eq(funcExp, GREATER)) return greatert(car(argsExp), car(cdr(argsExp))) ? T : NIL;
  else {
    SExp* funcValExp = getFuncValExp(funcExp->name);
    SExp* funcBodyExp = car(cdr(funcValExp));
    SExp* funcParamsExp = car(cdr(car(funcValExp)));
    //cout << "funcVal: " << output(funcValExp) << endl;
    //cout << "funcBody: " << output(funcBodyExp) << endl;
    //cout << "funcParams: " << output(funcParamsExp) << endl;
    if (funcValExp->type == Err) return funcValExp;
    try {
      return eval(funcBodyExp, addPairs(funcParamsExp, argsExp, aList));
    } catch (const char* s) {
      return new SExp(Error(s));
    }
  }

  return new SExp(Error("invalid apply sexp"));
}

SExp* evcon(SExp* exp, AList aList) {
  //string s = output(exp);
  //cout << "evcon: "<< s << endl;
  //string t = output(car(cadr);
  //cout << "caar: " << t << endl;
  if (eq(exp, NIL)) return new SExp(Error("no conditions evaluated to T"));
  else if (eq(eval(caar(exp),aList), T)) return eval(car(cdr(car(exp))), aList);
  else return evcon(cdr(exp), aList);
}

SExp* evlis(SExp* lisExp, AList aList) {
  //cout << "In Evlis with: " << output(lisExp) << endl;
  // TODO check to make sure that lisExp is a list
  if (eq(lisExp, NIL)) return NIL;
  if (lisExp->type != NonAtom) {
    //cout << "threw exception" << endl;
    //throw "cannot evaluate " + output(lisexp) + " as a list";
    return new SExp(Error("cannot evaluate " + output(lisExp) + " as a list"));
  }
  //SExp* rest;
  //try {
  //  rest = evlis(cdr(lisExp), aList);
  //  return cons(eval(car(lisExp), aList), rest);
  //} catch (string s) {
  //  throw;
  //  //return new SExp(Error(s));
  //}
  return cons(eval(car(lisExp), aList), evlis(cdr(lisExp), aList));
}


SExp* eval(SExp* exp, AList aList) {
  if (atom(exp)) {
    if (exp->type == Int) { return exp; }
    else if (eq(exp, T)) { return T; }
    else if (eq(exp, NIL)) { return NIL; }
    else if (in(exp, aList)) { return aList.at(exp->name); }
    else return new SExp(Error("unbound atom: " + exp->name));
  }

  else if (exp->type == NonAtom && atom(car(exp))) {
    if (eq(car(exp), QUOTE)) {
      // TODO make sure that the sexp is a list, with only two elements
      return cadr(exp);
    }
    else if (eq(car(exp), COND)) { return evcon(cdr(exp), aList); }
    else {
      //SExp* evaledParams;
      //try {
      //  evaledParams = evlis(cdr(exp), aList);
      //  return apply(car(exp), evaledParams, aList);
      //} catch(string s) {
      //  cout << "caught exc in eval" << endl;
      //  return new SExp(Error(s));
      //}
      SExp* res = apply(car(exp), evlis(cdr(exp), aList), aList);
      //cout << "res for " << output(exp) << " is: " << output(res) << endl;
      //return  apply(car(exp), evlis(cdr(exp), aList), aList);
      return res;
    }
  }


  return new SExp(Error("Not a lisp expression")); 
}

bool maybeAddDefun(SExp* maybeDefunExp) {
  if (maybeDefunExp->type != NonAtom) return false;
  if (!eq(car(maybeDefunExp), DEFUN)) return false;

  SExp* funcDefExp = car(cdr(maybeDefunExp));

  if (car(funcDefExp)->type == Symbol && car(cdr(funcDefExp))->type == NonAtom) {
    dList[car(funcDefExp)->name] = cdr(maybeDefunExp);
    return true;
  }

  return false;
}

SExp* eval(SExp* in) {
  if (maybeAddDefun(in)) {
    //cout << "Added Defun" << endl;
    return car(car(cdr(in)));
  }
	return eval(in, AList());
}


#endif
