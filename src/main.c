#include "compiler.h"

int main(int argc, char **argv)
{
  if (argc != 2) {
    error_at(token -> str, "don't collect number of parameter");
    return 1;
  }

  user_input = argv[1];
  token      = tokenize();
  program();

  // out put assembly
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // prologue
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  // generate code
  for (int i = 0; code[i]; ++i) {
    gen(code[i]);
    // pop assesment of assignment
    printf("  pop rax\n");
  }

  // epilogue
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");

  return 0;
}
