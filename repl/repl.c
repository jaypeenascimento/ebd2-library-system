#include "repl.h"

#include "../commands/commands.h"
#include "../structures/abb.h"
#include "../structures/heap.h"
#include "../utils.h"

#include <stdio.h>
#include <stdlib.h>

void flushStdin() {
  int c = 0;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void printHeader() {
  printf("Escolha uma das opções abaixo:\n");
  printf("[1] Carregar Categorias\n");
  printf("[2] Listar Categorias\n");
  printf("[3] Selecionar Categoria\n");
  printf("[4] Mostrar Top 1 livro mais vendido\n");
  printf("[5] Listar Top N livros mais vendidos\n");
  printf("[6] Registrar vendas:\n");
  printf("[0] Sair\n");
}

int read() {
  int n = 0;
  scanf("%d", &n);

  return n;
}

void REPL() {
  static ABB *root = NULL;
  Heap *heap = NULL;

  while (1) {
    clearScreen();
    printHeader();
    int input = read();

    switch (input) {
    case 1:
      abb_load_categories(&root, "data/categorias.csv");
      printf("Categorias carregadas com sucesso!\n");
      break;
    case 2:
      printf("Categorias disponíveis:\n");
      abb_list_categories(root);
      break;
    case 3:
      printf("Digite o nome da categoria desejada: ");
      char category[100];
      scanf("%s", category);

      char *filename = abb_return_file(root, category);

      heap = NULL;
      load_books(&heap, filename);
      printf("\nLivros da categoria \"%s\" foram carregados!\n", filename);
      break;
    case 4:
      Book *b = top_1_book(&heap);
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
      Book **topBooks = top_n_books(&heap, n, &out);
      if (topBooks == NULL) {
        break;
      }

      printf("\nTop %d livro mais vendido da categoria:\n", n);
      for (int i = 0; (size_t)i < out; i++) {
        printBook(topBooks[i]);
      }
      break;
    case 6:
      if (root == NULL) {
        printf("Categorias esta vazia, nao e possivel buscar livros\n");
        break;
      }

      if (heap == NULL) {
        printf("Heap esta vazia, nao e possivel buscar livros\n");
        break;
      }

      printf("\nEscreva o ISBN do livro: ");
      int isbn = 0;
      scanf("%d", &isbn);

      Heap *foundNode = heap_find_node_by_isbn(&heap, isbn);
      if (foundNode) {
        printf("\nLivro encontrado: \n");
        printBook(foundNode->book);
      } else {
        printf("\nNenhum livro foi encontrado com o isbn %d!\n", isbn);
        break;
      }

      printf("\nDigite a quantidade de vendas a registrar: ");
      int vendas = 0;
      scanf("%d", &vendas);

      // Add vendas to it's node index and book's value
      foundNode->index += vendas;
      foundNode->book->vendas += vendas;
      foundNode->book->estoque -= vendas;

      heapify_up(heap, foundNode);

      break;
    case 0:
      printf("Saindo...\n");
      abb_destroy(root);
      return;
    default:
      printf("Opção inválida. Tente novamente.\n");
    }

    printf("\n\nAperte qualquer tecla para prosseguir...");
    flushStdin();
    getchar();
  }
}
