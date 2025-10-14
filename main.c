#include "./repl/repl.h"

#include "./data.h"
#include "./structures/heap.h"
#include <stddef.h>
#include <stdio.h>

void testingHeap() {
  Book *book1 = CreateBook(123, "O lobo e a fera", "Pedro", 2015, 100, 2, 580);
  Book *book2 =
      CreateBook(13, "Oloquinho meu", "Lucas Neto", 2015, 100, 2, 200);
  Book *book3 =
      CreateBook(15, "Danca gatinho danca", "Rodrigo faro", 2015, 100, 2, 780);

  Book *book4 = CreateBook(17, "xose", "Rodrigo faro", 2015, 100, 2, 300);

  Heap *top = heap_create_node(book1);
  Heap *oloco = heap_create_node(book2);
  Heap *gatinho = heap_create_node(book3);
  Heap *xose = heap_create_node(book4);

  heap_insert(&top, oloco);
  // printf("TOP Index: %d\n", top->index);
  // printf("TOP->left (ptr): %p\n", top->left);
  // printf("TOP->left Index: %d\n", top->left->index);
  // printf("TOP->right (ptr): %p\n", top->right);

  heap_insert(&top, gatinho);
  // printf("TOP Index: %d\n", top->index);
  // printf("TOP->left (ptr): %p\n", top->left);
  // printf("TOP->left Index: %d\n", top->left->index);
  // printf("TOP->right (ptr): %p\n", top->right);
  // printf("TOP->right Index: %d\n", top->right->index);

  heap_insert(&top, xose);
  // printf("XOSEEEE\n\n");
  // printf("TOP Index: %d\n", top->index);
  // printf("TOP->left (ptr): %p\n", top->left);
  // printf("TOP->left Index: %d\n", top->left->index);
  // printf("TOP->right (ptr): %p\n", top->right);
  // printf("TOP->right Index: %d\n", top->right->index);

  Heap **seq = heap_traversal_sequence(&top);

  printf("Seq 1: %d\n", seq[0]->index);
  printf("Seq 2: %d\n", seq[1]->index);
  printf("Seq 3: %d\n", seq[2]->index);
  printf("Seq 4: %d\n", seq[3]->index);

  // Testing heap get top books
  size_t out = 0;
  Book **topBooks = heap_get_top_books(&top, 2, &out);

  printf("Out: %lu\n", out);

  printBook(topBooks[0]);
  printBook(topBooks[1]);
  printBook(topBooks[2]);

  printf("\n Finding book \n");
  Heap *foundNode = heap_find_node_by_isbn(&top, 13);
  if (foundNode) {
    printf("Found book: \n");
    printBook(foundNode->book);
  } else {
    printf("Book not found!\n");
  }

  // Add vendas to a Node
  // Should modify both index and node->vendas.
  foundNode->index += 1200;
  foundNode->book->vendas += 1200;

  printf("Updated book: \n");
  printBook(foundNode->book);

  printf("Heapifying up: \n");
  heapify_up(top, foundNode);

  printf("\n Printing sequence again: \n");
  Heap **updatedSeq = heap_traversal_sequence(&top);

  printf("Seq 1: %d\n", updatedSeq[0]->index);
  printf("Seq 2: %d\n", updatedSeq[1]->index);
  printf("Seq 3: %d\n", updatedSeq[2]->index);
  printf("Seq 4: %d\n", updatedSeq[3]->index);
}

int main() {
  REPL();
  return 0;
}
