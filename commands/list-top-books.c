#include "commands.h"

#include "../data.h"
#include "../structures/heap.h"
#include <stdio.h>

Book **top_n_books(Heap **heap, int n, size_t *foundCount) {
  if (heap == NULL || *heap == NULL) {
    printf("Nao foi possivel buscar top %d livros: Heap esta vazia!\n", n);
    return NULL;
  }

  size_t out = 0;
  Book **books = heap_get_top_books(heap, n, &out);

  if (out == 0) {
    printf(
        "Nao foi possivel buscar top %d livro: Nao foram encontrados livros!\n",
        n);
    return NULL;
  }

  if (out != (size_t)n) {
    printf("Foram encontrados menos livros do que o requisitado. Requisitado: "
           "%d, Encontrados: %lu",
           n, out);
  }

  *foundCount = out;
  return books;
}
