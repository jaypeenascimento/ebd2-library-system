#include "./repl/repl.h"
#include <stdio.h>

#include "./structures/abb.h"

int main() {
  printf("Bem vido ao sistema da biblioteca!\n");

  ABB *node = node_create("Testing");

  REPL();
  return 0;
}
