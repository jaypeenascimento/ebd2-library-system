#ifndef HEAP_H
#define HEAP_H

#include "../data.h"
#include <stddef.h>

typedef struct Heap {
  int index;
  Book *book;

  struct Heap *left;
  struct Heap *right;
} Heap;

// Node
Heap *heap_create_node(Book *book);
void heap_destroy_node(Heap *node);

// Heap
void heap_insert(Heap **root, Heap *node);
void heap_delete(Heap *root, Heap *node);
void heap_destroy();
void heapify_up(Heap *root, Heap *node);

Heap **heap_traversal_sequence(Heap **root);
Book **heap_get_top_books(Heap **root, int n, size_t *foundCount);
Heap *heap_find_node_by_isbn(Heap **root, int isbn);

#endif
