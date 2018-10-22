/*
 * =====================================================================================
 *
 *       Filename:  Output.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/21/2018 14:11:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "SExp.h"
#include <string>

using std::string;
using std::to_string;

bool findError(SExp* sexp, Error& err) {
  if (sexp->type == Err) {
    err.message = (sexp->err).message;
    return true;
  } else if (sexp->type == NonAtom) {
    Error branchErr;
    bool isBranchErr = findError(sexp->left, branchErr);
    if (!isBranchErr) { // if no error in left
      isBranchErr = findError(sexp->right, branchErr);
    }
    err.message = branchErr.message;
    return isBranchErr;
  } else {
    return false;
  }
}

string outputNoErrors(SExp* sexp) {
  switch (sexp->type) {
    case Int:
      return to_string(sexp->val);
    case Symbol:
      return sexp->name;
    case NonAtom:
      return "(" + outputNoErrors(sexp->left) + " . " + outputNoErrors(sexp->right) + ")";
    case Err:
      return "**internal error**";
  }
}

string output(SExp* sexp) {
  Error err;
  bool isErr = findError(sexp, err);
  if (isErr) {
    return err.message;
  }

  return outputNoErrors(sexp);
}
