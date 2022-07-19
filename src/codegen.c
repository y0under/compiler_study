#include "compiler.h"
/*
main(){
  return hoge(1, 2);
}
hoge(a, b){
  return a + b;
}
*/

int label_counter = 0;

char *register_name[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void gen_lval(Node *node )
{
  if (node -> kind != ND_LVAL)
    error_at(token -> str, "The lvalue of the assignment is not a variable.");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node -> offset);
  printf("  push rax\n");
}

void gen(Node *node)
{
  switch (node -> kind) {
    case ND_NUM:
      printf("  push %d\n", node -> val);
      return;

    case ND_LVAL:
      gen_lval(node);
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;

    case ND_ASSIGN:
      gen_lval(node -> lhs);
      gen(node -> rhs);

      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  mov [rax], rdi\n");
      printf("  push rdi\n");
      return;

    case ND_RETURN:
      gen(node -> lhs);
      printf("  pop rax\n");
      printf("  mov rsp, rbp\n");
      printf("  pop rbp\n");
      printf("  ret\n");
      return;

    case ND_IF: {
      int counter = label_counter;
      ++label_counter;
      gen(node -> condition);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lelse%03d\n", counter);
      gen(node -> then);
      printf("  jmp .Lend%03d\n", counter);
      printf(".Lelse%03d:\n", counter);
      if (node -> else_proc) {
        gen(node -> else_proc);
      }
      printf(".Lend%03d:\n", counter);
      return;
    }

    case ND_WHILE: {
      int counter = label_counter;
      ++label_counter;
      printf(".Lbegin%03d:\n", counter);
      gen(node -> condition);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%03d\n", counter);
      if (node -> body)
        gen(node -> body);
      printf("  jmp .Lbegin%03d\n", counter);
      printf(".Lend%03d:\n", counter);
      return;
    }

    case ND_FOR: {
      int counter = label_counter;
      ++label_counter;
      if (node -> init)
        gen(node -> init);
      printf(".Lbegin%03d:\n", counter);
      if (node -> condition)
        gen(node -> condition);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%03d\n", counter);
      if (node -> body)
        gen(node -> body);
      if (node -> change)
        gen(node -> change);
      printf("  jmp .Lbegin%03d\n", counter);
      printf(".Lend%03d:\n", counter);
      return;
    }

    case ND_BLOCK: {
      for (int i = 0; i < node -> stmts -> len; ++i) {
        gen(node -> stmts -> data[i]);
      }
      return;
    }

    case ND_FUNC_CALL: {
      for (int i = node -> args -> len - 1; i > -1; --i) {
        gen(node -> args -> data[i]);
        printf("  pop rax\n");
        printf("  mov %s, rax\n", register_name[i]);
      }
      printf("  mov al, 0\n");
      printf("  call %s\n", node -> name);
      printf("  push rax\n");

      return;
    }

    case ND_FUNC: {
      printf("\n  %s:\n", node -> name);

      // prologue
      printf("  push rbp\n");
      printf("  mov rbp, rsp\n\n");
      //printf("  sub rsp, 208\n");
      for (int i = 0; i < node -> args -> len; ++i)
        printf("  push %s\n", register_name[i]);

      gen(node -> lhs);

      // epilogue
      printf("\n  mov rsp, rbp\n");
      printf("  pop rbp\n");
      printf("  ret\n");

      return;
    }

    case ND_ADDR: {
      gen_lval(node -> lhs);
      return;
    }

    case ND_DEREF: {
      gen(node -> lhs);
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;
    }

    default:
      break;
  }

  gen(node -> lhs);
  gen(node -> rhs);

  // preparea for calculate
  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node -> kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf(  "cqo\n");
      printf("  idiv rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NE:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
    default:
      break;
  }

  printf("  push rax\n");
}

