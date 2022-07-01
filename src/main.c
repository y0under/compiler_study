#include "compiler.h"

int main(int argc, char **argv)
{
  if (argc != 2) {
    error_at(token -> str, "don't collect number of parameter");
    return 1;
  }

  user_input = argv[1];
  token      = tokenize();
  Node *node = expr();

  // out put assembly
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // generate code from abstruct tree
  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
