#ifndef COMPILER_SRC_H
#define COMPILER_SRC_H

struct Type
{
  enum {INT, PTR} ty;
  struct Type *ptr_to;
};

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
  Node *condition; // if and for condition
  Node *then;      // if condition is true
  Node *else_proc; // else processing
  Node *body;      // body of while, for
  Node *init;      // for init
  Node *change;    // for change
  Vector *stmts;   // statement for {} block
  Vector *args;    // for arguments of function
  char *name;      // function name
  int val;         // only kind type is ND_NUM
  int offset;      // only kind type is ND_LVAL 
  Type ty;
};

struct LVar
{
  LVar *next;
  char *name;
  int len;
  int offset; // offset from RBP
};

struct Function
{
  char *name;
  Node *node;
};

#endif
