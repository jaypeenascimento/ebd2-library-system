#include "abb.h"
#include "../csv/csvReader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ABB {
  char *category;
  char *filename;
  struct ABB *left;
  struct ABB *right;
};

char *strdup(const char *str) {
  char *p = malloc(strlen(str) + 1);
  if (p) {
    strcpy(p, str);
  }
  return p;
}

ABB *node_create(const char *cat, const char *filename) {
  ABB *n = malloc(sizeof(*n));
  if (!n)
    return NULL;
  n->category = strdup(cat ? cat : "");
  n->filename = strdup(filename ? filename : "");
  if (!n->category || !n->filename) {
    free(n->category);
    free(n->filename);
    free(n);
    return NULL;
  }
  n->left = n->right = NULL;
  return n;
}

void node_destroy(ABB *node) {
  if (!node)
    return;
  node_destroy(node->left);
  node_destroy(node->right);
  free(node->category);
  free(node->filename);
  free(node);
}

void abb_destroy(ABB *root) { node_destroy(root); }

int abb_insert(ABB **root, const char *name, const char *filename) {
  if (!root || !name || !filename)
    return -1;
  if (!*root) {
    *root = node_create(name, filename);
    return *root ? 0 : -1;
  }
  ABB *cur = *root;
  while (1) {
    int cmp = strcmp(name, cur->category);
    if (cmp == 0)
      return 0; // já existe
    if (cmp < 0) {
      if (!cur->left) {
        cur->left = node_create(name, filename);
        return cur->left ? 0 : -1;
      }
      cur = cur->left;
    } else {
      if (!cur->right) {
        cur->right = node_create(name, filename);
        return cur->right ? 0 : -1;
      }
      cur = cur->right;
    }
  }
}

int abb_load_categories(ABB **root, const char *path) {
  if (!root || !path)
    return -1;
  CSVReader *r = csv_open(path);
  if (!r)
    return -1;
  size_t cols = 0;
  int inserted = 0;
  int first = 1;
  while (1) {
    char **row = csv_read_row(r, &cols);
    if (!row)
      break;

    if (cols >= 3) {

      if (first) {
        first = 0;
        if (strcmp(row[1], "nome_categoria") == 0) {
          // csv_free_fields(row, cols);
          continue;
        }
      }
      const char *name = row[1];
      const char *file = row[2];
      if (name && name[0] != '\0' && file && file[0] != '\0') {
        if (abb_insert(root, name, file) == 0)
          inserted++;
      }
    }
    // csv_free_fields(row, cols);
  }
  csv_close(r);
  return inserted;
}

void abb_list_categories(ABB *node) {
  if (!node) {
    return;
  }
  abb_list_categories(node->left);
  printf("%s\n", node->category);
  abb_list_categories(node->right);
}

int abb_is_empty(const ABB *root) { return root == NULL ? 1 : 0; }

char *abb_return_file(ABB *node, const char *name) {
  if (!node || !name)
    return NULL;
  int cmp = strcmp(name, node->category);
  if (cmp == 0) {
    return strdup(node->filename);
  } else if (cmp < 0) {
    return abb_return_file(node->left, name);
  } else {
    return abb_return_file(node->right, name);
  }
}
