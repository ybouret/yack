.eval;


add  : mult ('+' mult)* ; // left association
mult : pow ('*'  pow)* ; // left association
pow  : atom ('^' pow)? ; // right association
atom : ID | INT | '(' add ')' ; // recursion

#include "lex.g"
