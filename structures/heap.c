#include "heap.h"

#include <stdbool.h>
#include <stddef.h>
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

// Free the node and it's book.
void heap_destroy_node(Heap *node) {
  if (node == NULL) {
    return;
  }

  Book *book = node->book;
  free(book);
  free(node);
}

int node_index(Heap *node) {
  if (node == NULL) {
    return -1;
  }

  return node->index;
}

bool invariant(Heap *node) {
  if (node == NULL) {
    return true;
  }

  int left = node_index(node->left);
  int right = node_index(node->right);

  if (node->left && node->index < left) {
    return false;
  }
  if (node->right && node->index < right) {
    return false;
  }

  return invariant(node->left) && invariant(node->right);
}

Heap *find_parent(Heap *root, Heap *node) {
  if (root == NULL || root == node) {
    return NULL;
  }

  Heap *queue[BOOKS_MAX];
  int head = 0, tail = 0;
  queue[tail++] = root;

  while (head < tail) {
    Heap *current = queue[head++];
    if (current->left == node || current->right == node) {
      return current;
    }

    if (current->left) {
      queue[tail++] = current->left;
    }
    if (current->right) {
      queue[tail++] = current->right;
    }
  }
  return NULL;
}

void swap_nodes_data(Heap *a, Heap *b) {
  if (!a || !b)
    return;

  // Use temporary variables to hold the data from node 'a'.
  Book *temp_book = a->book;
  int temp_index = a->index;

  // Copy data from 'b' to 'a'.
  a->book = b->book;
  a->index = b->index;

  // Copy the original data from 'a' (now in temp) to 'b'.
  b->book = temp_book;
  b->index = temp_index;
}

void heapify_up(Heap *root, Heap *node) {
  if (node == root) {
    return;
  }

  Heap *parent = find_parent(root, node);

  // Check if the heap property is violated.
  // The child's index should NOT be greater than the parent's.
  if (parent != NULL && node->index > parent->index) {
    // If it is, swap their data (the book and the index).
    swap_nodes_data(node, parent);

    // Recursive Step: Continue the process from the parent's position,
    // as it now holds the smaller value.
    heapify_up(root, parent);
  }
}

void heap_insert(Heap **root, Heap *node) {
  if (node == NULL)
    return;

  if (*root == NULL) {
    *root = node;
    return;
  }

  // Start the recursive process to find the structural insertion point.
  Heap *queue[BOOKS_MAX];
  int head = 0, tail = 0;
  queue[tail++] = *root;

  Heap *parent = NULL;
  while (head < tail) {
    parent = queue[head++];

    if (parent->left == NULL) {
      parent->left = node;
      break;
    } else {
      queue[tail++] = parent->left;
    }

    if (parent->right == NULL) {
      parent->right = node;
      break;
    } else {
      queue[tail++] = parent->right;
    }
  }

  heapify_up(*root, node);
}

Heap **heap_traversal_sequence(Heap **root) {
  Heap **queue = calloc(BOOKS_MAX, sizeof(Book *));
  if (queue == NULL) {
    printf("Couldn't traversal heap: malloc result buffer failed\n");
    return NULL;
  }
  int head = 0, tail = 1;

  queue[0] = *root;

  Heap *curr = NULL;
  while (head < tail) {
    curr = queue[head++];

    if (curr->left != NULL) {
      queue[tail++] = curr->left;
    }

    if (curr->right != NULL) {
      queue[tail++] = curr->right;
    }
  }

  queue[tail] = NULL;
  return queue;
}

Book **heap_get_top_books(Heap **root, int n, size_t *foundCount) {
  Heap **nodes = heap_traversal_sequence(root);
  Book **books = malloc(sizeof(Book *) * n);
  if (books == NULL) {
    printf("Couldn't get top books: Failed malloc for Books buffer");
    return NULL;
  }

  // Iterating
  for (int i = 0; i < n; i++) {
    Heap *curr = nodes[i];
    if (curr == NULL) {
      // Finished list.
      break;
    }

    books[i] = curr->book;
    *foundCount = *foundCount + 1;
  }

  free(nodes);
  return books;
}

Heap *heap_find_node_by_isbn(Heap **root, int isbn) {
  if (root == NULL) {
    printf("Couldn't find node by isbn: Root is NULL");
  }

  Heap *queue[BOOKS_MAX];
  int head = 0, tail = 1;

  queue[0] = *root;

  Heap *curr = NULL;
  while (head < tail) {
    curr = queue[head++];

    // Check if current's node book's matches the ispb.
    if (curr->book->isbn == isbn) {
      return curr;
    }

    if (curr->left != NULL) {
      queue[tail++] = curr->left;
    }

    if (curr->right != NULL) {
      queue[tail++] = curr->right;
    }
  }

  // Couldn't find book with ISBN
  return NULL;
};
