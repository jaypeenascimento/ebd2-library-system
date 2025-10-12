#include "repl.h"
#include "../commands/commands.h"
#include "../structures/heap.h"

#include <stdio.h>
#include <stdlib.h>

void printHeader() {
  printf("\n");

  printf("Escolha uma das opções abaixo:\n");
  printf("[1] Carregar Categorias\n");
  printf("[2] Listar Categorias\n");
  printf("[3] Selecionar Categoria\n");
  printf("[4] Mostrar Top 1 livro mais vendido\n");
  printf("[5] Listar Top N livros mais vendidos\n");
  printf("[6] Listar registrar venda:\n");
  printf("[0] Sair\n");
}

void process(Heap **heap, int option) {
  switch (option) {
  case 1:
    break;
  case 2:
    break;
  case 3:
    // TODO: Printar lista de categorias atual
    // TODO: Passar arquivo da categoria para a funcao abaixo:
    load_books(heap, "romance.csv");
    printf("Livros da categoria foram carregados!");
    break;
  case 4:
    Book *b = top_1_book(heap);

    printf("Book found by top_1_book: \n\n");
    printBook(b);
    break;
  case 5:
    // int n;
    //
    // printf("Digite o valor de N: ");
    // scanf("%d", &n);
    //

    break;
  case 6:
    break;
  case 0:
    printf("Saindo...\n");
    exit(0);
  default:
    printf("Opção inválida. Tente novamente.\n");
  }
}

int read() {
  int n = 0;
  scanf("%d", &n);

  return n;
}

void REPL() {
  Heap *heap = NULL;

  while (1) {
    printHeader();
    int input = read();
    process(&heap, input);
  }
}
