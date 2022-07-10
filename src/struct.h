#ifndef COMPILER_SRC_H
#define COMPILER_SRC_H

struct Token
{
  TokenKind kind;  // type of token
  Token     *next; // next token
  int       val;   // number of token when kind is TK_NUM
  char      *str;  // string of token
  int       len;   // length of token
};

struct Node
{
  NodeKind kind;   // type of node
  Node *lhs;       // left-hand side
  Node *rhs;       // right-hand size
  Node *condition; // if condition
  Node *then;      // if condition is true
  Node *else_proc; // else processing
  Node *body;      // body of while
  int val;         // only kind type is ND_NUM
  int offset;      // only kind type is ND_LVAL 
};

struct LVar
{
  LVar *next;
  char *name;
  int len;
  int offset; // offset from RBP
};

#endif
