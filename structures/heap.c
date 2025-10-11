#include "heap.h"

#include <stdbool.h>
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

  Heap *queue[1024];
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
  Heap *queue[1024];
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
