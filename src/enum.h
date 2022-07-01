#ifndef COMPILER_SRC_ENUM_H
#define COMPILER_SRC_ENUM_H

typedef enum
{
  TK_RESERVED,  // either +, -, *, /, (, )
  TK_NUM,       // number
  TK_EOF,       // end of input value
} TokenKind;

typedef enum
{
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_NUM, // number
} NodeKind;

#endif
