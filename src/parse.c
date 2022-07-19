#include "compiler.h"

// indicate now token
Token *token;

// input program
char *user_input;

LVar *local_var;

const char *kreturn      = "return";
const char *kif          = "if";
const char *kelse        = "else";
const char *kwhile       = "while";
const char *kfor         = "for";
const char *kbrace_left  = "{";
const char *kbrace_right = "}";

bool is_expect_token(char *expect_token)
{
  return !memcmp(token -> str, expect_token, token -> len);
}

void expect(char *op)
{
  if (token -> kind != TK_RESERVED
      || strlen(op) != token -> len
      || memcmp(token -> str, op, token -> len))
    error_at(token -> str, "not '%s'", op);
  token = token -> next;
}

int expect_number()
{
  if (token -> kind != TK_NUM)
    error_at(token -> str, "not a number");
  int val = token -> val;
  token = token -> next;
  return val;
}

bool at_eof()
{
  return token -> kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
  Token *tok  = calloc(1, sizeof(Token));
  tok -> kind = kind;
  tok -> str  = str;
  tok -> len  = len;
  cur -> next = tok;
  return tok;
}

bool start_swith(const char *p, const char *q)
{
  return memcmp(p, q, strlen(q)) == 0;
}

bool is_alpha_or_underscore(const char *p)
{
  return ('a' <= *p && *p <= 'z')
    || ('A' <= *p && *p <= 'Z')
    || ('_' == *p);
}

bool is_alpha_or_under_or_num(const char *p)
{
  return is_alpha_or_underscore(p)
    || ('0' <= *p && *p <= '9');
}


bool is_reserved_keyword(
    const char *p, const char *keyword, const size_t key_length)
{
  return 0 == strncmp(p, keyword, key_length)
    && !is_alpha_or_under_or_num(p + key_length);
}

bool is_tk_reserved(const char *p)
{
  return *p == '+' || *p == '-'
      || *p == '*' || *p == '/'
      || *p == '(' || *p == ')'
      || *p == '<' || *p == '>'
      || *p == ';' || *p == '='
      || *p == '{' || *p == '}'
      || *p == ',' || *p == '&';
}

bool is_two_char_operation(const char *p)
{
  return start_swith(p, "==")
      || start_swith(p, "!=")
      || start_swith(p, "<=")
      || start_swith(p, ">=");
}

Token *tokenize()
{
  char *p = user_input;
  Token head;
  head.next  = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (is_two_char_operation(p)) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p += 2;
      continue;
    }

    if (is_tk_reserved(p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    // return
    {
      size_t key_length = strlen(kreturn);
      if (is_reserved_keyword(p, kreturn, key_length)) {
        cur = new_token(TK_RETURN, cur, p, key_length);
        p += key_length;
        cur -> len = key_length;
        continue;
      }
    }

    // if
    {
      size_t key_length = strlen(kif);
      if (is_reserved_keyword(p, kif, key_length)) {
        cur = new_token(TK_IF, cur, p, key_length);
        p += key_length;
        cur -> len = key_length;
          continue;
      }
    }

    // else
    {
      size_t key_length = strlen(kelse);
      if (is_reserved_keyword(p, kelse, key_length)) {
        cur = new_token(TK_ELSE, cur, p, key_length);
        p += key_length;
        cur -> len = key_length;
        continue;
      }
    }

    // while
    {
      size_t key_length = strlen(kwhile);
      if (is_reserved_keyword(p, kwhile, key_length)) {
        cur = new_token(TK_WHILE, cur, p, key_length);
        p += key_length;
        cur -> len = key_length;
        continue;
      }
    }

    // for
    {
      size_t key_length = strlen(kfor);
      if (is_reserved_keyword(p, kfor, key_length)) {
        cur = new_token(TK_FOR, cur, p, key_length);
        p += key_length;
        cur -> len = key_length;
        continue;
      }
    }

    // variable name or function name
    if (is_alpha_or_underscore(p)) {
      int len = 1;
      while (is_alpha_or_under_or_num(p + len))
        ++len;
      cur = new_token(TK_IDENT, cur, p, len);
      p += len;
      cur -> len = len;
      continue;
    }

    if (isdigit(*p)) {
      cur        = new_token(TK_NUM, cur, p, 0);
      char *q    = p;
      cur -> val = strtol(p, &p, 10);
      cur -> len = p - q;
      continue;
    }

    error_at(token -> str, "cannot tokenize");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

/*
 * what: determin if it is a reserved token
 */
bool consume(const char *op)
{
  if ((token -> kind != TK_RESERVED
      && token -> kind != TK_RETURN
      && token -> kind != TK_IF
      && token -> kind != TK_ELSE
      && token -> kind != TK_WHILE
      && token -> kind != TK_FOR)
      || strlen(op) != token -> len
      || memcmp(token -> str, op, token -> len))
    return false;
  token = token -> next;
  return true;
}

/*
 * if op is ident, return pointer
 */
Token *consume_ident()
{
  if (token -> kind != TK_IDENT)
    return NULL;
  Token *ident_token = token;
  token = token -> next;
  return ident_token;
}

/*
 * make node of operation
 */
Node *new_node_operation(NodeKind kind, Node *lhs, Node *rhs)
{
  Node *node   = calloc(1, sizeof(Node));
  node -> kind = kind;
  node -> lhs  = lhs;
  node -> rhs  = rhs;
  return node;
}

/*
 * make node of number
 */
Node *new_node_number(int val)
{
  Node *node = calloc(1, sizeof(Node));
  node -> kind = ND_NUM;
  node -> val  = val;
  return node;
}

// the head of the definition of the EBNF
Node *code[100];

/*
 * program = func*
 */
void program()
{
  int i = 0;
  while (!at_eof()){
    code[i++] = func();
  }
  code[i] = NULL;
}

/*
 * func = ident "(" stmt* ")" stmt
 */
Node *func()
{
  Token *tok = consume_ident();
  if (!tok)
    error_at(token -> str, "not identity");

  Node   *node = calloc(1, sizeof(Node));
  node -> kind = ND_FUNC;
  node -> name = calloc((tok -> len) + 1, sizeof(char));
  memcpy(node -> name, tok -> str, tok -> len);
  node -> name[tok -> len] = '\0';

  // arguments of function
  {
    expect("(");
    node -> args = new_vec();
    while (!consume(")")) {
      consume(",");
      Node *arg = expr();
      vec_push(node -> args, arg);
      if (node -> args -> len > 6)
        error_at(token -> str, "number of args for function exceed 6.");
    }
  }

  // body of function
  node -> lhs = stmt();

  return node;
}

/*
 * stmt = expr ";"
 *      | "{" stmt "}"
 *      | "return" expr ";"
 *      | "if" "(" expr ")" stmt ("else" stmt)?
 *      | "while" "(" expr ")" stmt
 *      | "for" "(" expr? ";" expr? ";" expr? ")" stmt
 */
Node *stmt()
{
  Node *node;

  // {}
  if (consume(kbrace_left)) {
    if (!consume(kbrace_right)) {
      Vector *stmts = new_vec();
      node = calloc(1, sizeof(Node));
      node -> kind = ND_BLOCK;
      while (!consume("}"))
        vec_push(stmts, stmt());
      node -> stmts = stmts;
    }
  }
  // return statement
  else if (consume(kreturn)) {
    node = calloc(1, sizeof(Node));
    node -> kind = ND_RETURN;
    node -> lhs = expr();
    expect(";");
  }
  // if statement
  else if (consume(kif)) {
    expect("(");
    node = calloc(1, sizeof(Node));
    node -> kind      = ND_IF;
    node -> condition = expr();
    expect(")");
    node -> then = stmt();
    if (consume(kelse)) {
      node -> else_proc = stmt();
    }
  }
  // while statement
  else if (consume(kwhile)) {
    expect("(");
    node = calloc(1, sizeof(Node));
    node -> kind      = ND_WHILE;
    node -> condition = expr();
    expect(")");
    if (!consume(";"))
      node -> body = stmt();
  }
  // for statement
  else if (consume(kfor)) {
    expect("(");
    node = calloc(1, sizeof(Node));
    node -> kind = ND_FOR;
    if (!consume(";")) {
      node -> init = expr();
      expect(";");
    }
    if (!consume(";")) {
      node -> condition = expr();
      expect(";");
    }
    if (!consume(")")) {
      node -> change = expr();
      expect(")");
    }
    if (!consume(";"))
      node -> body = stmt();
  }
  else {
    node = expr();
    expect(";");
  }

  return node;
}

/*
 * expr = assign
 */
Node *expr()
{
  return assign();
}

/*
 * assign = equality ("=" assign)?
 */
Node *assign()
{
  Node *node = equality();
  if (consume("="))
    node = new_node_operation(ND_ASSIGN, node, assign());
  return node;
}

/*
 * equality = relational ("==" | "!=" relational)*
 */
Node *equality()
{
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node_operation(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node_operation(ND_NE, node, relational());
    else
      return node;
  }
}

/*
 * relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 */
Node *relational()
{
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_node_operation(ND_LT, node, add());
    else if (consume("<="))
      node = new_node_operation(ND_LE, node, add());
    else if (consume(">"))
      node = new_node_operation(ND_LT, add(), node);
    else if (consume(">="))
      node = new_node_operation(ND_LE, add(), node);
    else
      return node;
  }
}
/*
 * add = mul ("+" mul | "-" mul)*
 */
Node *add()
{
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node_operation(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node_operation(ND_SUB, node, mul());
    else
      return node;
  }
}

/*
 * mul = unary ("*" unary | "/" unary)*
 */
Node *mul()
{
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node_operation(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node_operation(ND_DIV, node, unary());
    else
      return node;
  }
}

/*
 * unary = ("+" | "-")? primary
 *       | ("*" | "&") unary
 */
Node *unary()
{
  if (consume("+"))
    return primary();
  if (consume("-"))
    // replace "-x" to "0-x"
    return new_node_operation(ND_SUB, new_node_number(0), primary());
  if (consume("*"))
    return new_node_operation(ND_DEREF, unary(), NULL);
  if (consume("&"))
    return new_node_operation(ND_ADDR, unary(), NULL);
  return primary();
}

/*
 * if tok is registed variable name, return variable ptr of the name.
 * if tok is not registed, return NULL.
 */
LVar *find_lvar(Token *tok)
{
  for (LVar *var = local_var; var; var = var -> next)
    if ((var -> len == tok -> len)
        && !memcmp(tok -> str, var -> name, var -> len))
      return var;
  return NULL;
}

void regist_lvar(const Token *const tok, LVar **lvar)
{
  *lvar = calloc(1, sizeof(LVar));

  (*lvar) -> next   = local_var;
  (*lvar) -> name   = tok -> str;
  (*lvar) -> len    = tok -> len;
  (*lvar) -> offset = (local_var) ? local_var -> offset + 8 : 8;

  local_var = *lvar;
}

/*
 * primary = num | ident | "(" expr ")"
 */ 
Node *primary()
{
  // expect ()
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  Token *tok = consume_ident();

  // number
  if (!tok)
    return new_node_number(expect_number());

  // variable
  if (!consume("(")) {
    Node *node   = calloc(1, sizeof(Node));
    node -> kind = ND_LVAL;

    LVar *lvar = find_lvar(tok);
    if (lvar)
      node -> offset = lvar -> offset;
    else {
      regist_lvar(tok, &lvar);
      node -> offset = local_var -> offset;
    }
    return node;
  }

  // call function
  {
    Node *node   = calloc(1, sizeof(Node));
    node -> kind = ND_FUNC_CALL;
    node -> name = calloc((tok -> len) + 1, sizeof(char));
    memcpy(node -> name, tok -> str, tok -> len);
    node -> name[tok -> len] = '\0';
    node -> args = new_vec();

    while (!consume(")")) {
      consume(",");
      Node *arg = expr();
      vec_push(node -> args, arg);
      if (node -> args -> len > 6)
        error_at(token -> str, "number of args for function exceed 6.");
    }
    return node;
  }

  error_at("error:", "failed primary");
  return NULL; // dummy
}

// the end of the definition of the EBNF
