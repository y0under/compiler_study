#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enum.h"

/*
 * prototype
 */

// struct
typedef struct Node Node;
typedef struct Token Token;

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
void program();
void error_at(char *, char *, ...);
void expect(char *);
void gen(Node *);
int expect_number();
bool consume(char *);
bool at_eof();
bool start_swith(char *, char *);
bool is_tk_reserved(char *);
bool is_two_char_operation(char *);

// value
extern char *user_input;
extern Token *token;
extern Node *code[100];
