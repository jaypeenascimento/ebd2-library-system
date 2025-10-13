#include "commands.h"

#include <stdio.h>

#include "../csv/csvReader.h"
#include "../structures/heap.h"

Book **read_books(char *path) {
  Book **books = ReadBooksFromCategory(path);
  if (books == NULL) {
    printf("Failed to load books from selected category: Cannot read books "
           "from category provided: \"%s\"",
           path);
    return NULL;
  }

  return books;
}

void load_books(Heap **heap, char *filename) {
  Book **books = read_books(filename);
  if (!books)
    return;

  for (int i = 0; books[i + 1] != NULL; i++) {
    printf("I: %d", i);
    Book *curr = books[i];

    Heap *node = heap_create_node(curr);
    heap_insert(heap, node);

    // Debuging:
    // printBook(curr);
  }
}
