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

void heapify_down(Heap *node) {
  if (node == NULL) {
    return;
  }

  Heap *largest = node;
  Heap *left = node->left;
  Heap *right = node->right;

  if (left != NULL && left->index > largest->index) {
    largest = left;
  }
  if (right != NULL && right->index > largest->index) {
    largest = right;
  }

  if (largest != node) {
    swap_nodes_data(node, largest);
    heapify_down(largest);
  }
}

Heap *heap_extract_max_node(Heap **root) {
  if (root == NULL || *root == NULL)
    return NULL;

  Heap *max_node = *root;

  // 1 - Heap have one node
  if (max_node->left == NULL && max_node->right == NULL) {
    *root = NULL;
    return max_node;
  }

  // 2 - Heap have multiple nodes
  // Find the last node in the tree using a level-order traversal.
  Heap *last_node = NULL;
  Heap *queue[1024]; // Assuming a max size
  int head = 0, tail = 0;
  queue[tail++] = *root;
  while (head < tail) {
    last_node = queue[head++];
    if (last_node->left)
      queue[tail++] = last_node->left;
    if (last_node->right)
      queue[tail++] = last_node->right;
  }

  // Find the parent of the last node so we can detach it.
  Heap *parent_of_last = find_parent(*root, last_node);

  // Detach the last node from the tree.
  if (parent_of_last->right == last_node) {
    parent_of_last->right = NULL;
  } else {
    parent_of_last->left = NULL;
  }

  // Last node is now the new root
  *root = last_node;
  last_node->left = max_node->left;
  last_node->right = max_node->right;

  heapify_down(*root);

  return max_node;
}

Heap *heap_deep_copy(Heap *original) {
  if (original == NULL) {
    return NULL;
  }
  Heap *copy_node = malloc(sizeof(Heap));
  if (copy_node == NULL)
    return NULL;

  copy_node->index = original->index;
  copy_node->book = original->book;
  copy_node->left = heap_deep_copy(original->left);
  copy_node->right = heap_deep_copy(original->right);

  return copy_node;
}

void heap_destroy_tree(Heap *node) {
  if (node == NULL)
    return;
  heap_destroy_tree(node->left);
  heap_destroy_tree(node->right);
  free(node);
}

Heap **heap_get_sorted_array(Heap *root) {
  Heap *heap_copy = heap_deep_copy(root);
  if (heap_copy == NULL) {
    printf("Error: Failed to create a copy of the heap for sorting.\n");
    return NULL;
  }

  size_t count = 0;
  Heap **sorted_array = malloc(BOOKS_MAX * sizeof(Heap *));
  if (sorted_array == NULL) {
    printf("Error: Failed to allocate memory for sorted array.\n");
    heap_destroy_tree(heap_copy);
    return NULL;
  }

  while (heap_copy != NULL) {
    Heap *max_node = heap_extract_max_node(&heap_copy);
    if (max_node) {
      sorted_array[count++] = max_node;
    }
  }

  sorted_array[count] = NULL;
  return sorted_array;
}

Book **heap_get_top_books(Heap **root, int n, size_t *foundCount) {
  Heap **nodes = heap_get_sorted_array(*root);
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
