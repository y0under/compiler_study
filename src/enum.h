#ifndef COMPILER_SRC_ENUM_H
#define COMPILER_SRC_ENUM_H

typedef enum
{
  // TK_RESERVED: +, -, *, /, (, ), ;, =
  //   ==, <=, !=, >=, {, }
  TK_RESERVED,
  TK_IDENT,    // recognizer
  TK_NUM,      // number
  TK_EOF,      // end of input value
  TK_RETURN,   // return statement
  TK_IF,       // if statement
  TK_ELSE,     // else statement
  TK_WHILE,    // while statement
  TK_FOR,      // for statement
  TK_INT,      // type int
} TokenKind;

typedef enum
{
  ND_ADD,    // +
  ND_SUB,    // -
  ND_MUL,    // *
  ND_DIV,    // /
  ND_ASSIGN, // =
  ND_LVAL,   // variable of local
  ND_EQ,     // ==
  ND_NE,     // !=
  ND_LT,     // <
  ND_LE,     // <=
  ND_SEM,    // ;
  ND_NUM,    // number
  ND_RETURN,
  ND_IF,
  ND_ELSE,
  ND_WHILE,
  ND_FOR,
  ND_BLOCK,     // {}
  ND_FUNC_CALL, // function call
  ND_FUNC,      // definition of function
  ND_ADDR,      // address of variable
  ND_DEREF,     // dereference
} NodeKind;

#endif
