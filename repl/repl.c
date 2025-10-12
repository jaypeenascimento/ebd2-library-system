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

    char *filename = "romance.csv";

    load_books(heap, filename);
    printf("\nLivros da categoria \"%s\" foram carregados!\n", filename);
    break;
  case 4:
    Book *b = top_1_book(heap);
    if (b == NULL) {
      printf(
          "Nenhuma categoria foi selecionada ou não há livros na categoria.");
      break;
    }

    printf("\nTop 1 livro mais vendido da categoria:\n");
    printBook(b);
    break;
  case 5:
    int n = 0;
    printf("Digite quantos livros devem ser listados: ");
    scanf("%d", &n);

    size_t out = 0;
    Book **topBooks = top_n_books(heap, n, &out);
    if (topBooks == NULL) {
      break;
    }

    printf("\nTop %d livro mais vendido da categoria:\n", n);
    for (int i = 0; (size_t)i < out; i++) {
      printBook(topBooks[i]);
    }
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
