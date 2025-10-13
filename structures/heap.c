#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

Heap *heap_create_node(Book *book) {
  Heap *h = malloc(sizeof(Heap));
  if (h == NULL) {
    printf("Failed to create HEAP: Failed to malloc Heap! Returning NULL.");
    return NULL;
  }

  if (book == NULL) {
    printf("Failed to create HEAP: Provided book pointer is NULL! Returning "
           "NULL.");
    return NULL;
  }

  h->index = book->vendas;
  h->book = book;
  h->left = NULL;
  h->right = NULL;

  return h;
}