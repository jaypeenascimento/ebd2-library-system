#ifndef HEAP_H
#define HEAP_H

#include "../data.h"

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
void heap_insert(Heap *root, Heap *node);
void heap_delete(Heap *root, Heap *node);
void heap_destroy();

#endif
