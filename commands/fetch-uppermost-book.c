#include "./commands.h"

#include "../data.h"
#include "../structures/heap.h"
#include <stdio.h>

Book *top_1_book(Heap **heap) {
  if (heap == NULL || *heap == NULL) {
    return NULL;
  }

  size_t out = 0;
  Book **books = heap_get_top_books(heap, 1, &out);

  if (out == 1) {
    return books[0];
  }

  // There was an error if out is != 1.
  // Either more than 1 book was caught or zero.
  return NULL;
}
