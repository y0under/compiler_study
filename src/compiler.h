#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enum.h"
#include "vector.h"

/*
 * prototype
 */

// struct
typedef struct Node  Node;
typedef struct Token Token;
typedef struct LVar  LVar;

#include "struct.h"

// enum

// function
Node *new_node_operation(NodeKind, Node*, Node *);
Node *new_node_number(int);
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
Token *new_token(TokenKind, Token *, char *, int);
Token *tokenize();
Token *consume_ident();
LVar *find_lvar(Token *);
void program();
void error_at(char *, char *, ...);
void expect(char *);
void gen(Node *);
void regist_lvar(const Token *const, LVar *);
int expect_number();
bool consume(const char *);
bool at_eof();
bool start_swith(const char *, const char *);
bool is_tk_reserved(const char *);
bool is_two_char_operation(const char *);
bool is_alpha_or_underscore(const char *);
bool is_alpha_or_under_or_num(const char *);

// value
extern char *user_input;
extern int label_counter;
extern Token *token;
extern Node *code[100];
