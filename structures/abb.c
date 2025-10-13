#include "abb.h"
#include "../csv/csvReader.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct ABB {
    char *category;
    struct ABB *left;
    struct ABB *right;
};

static ABB *node_create(const char *cat) {
    ABB *n = malloc(sizeof(*n));
    if (!n) return NULL;
    n->category = strdup(cat ? cat : "");
    if (!n->category) { free(n); return NULL; }
    n->left = n->right = NULL;
    return n;
}

static void node_destroy(ABB *node) {
    if (!node) return;
    node_destroy(node->left);
    node_destroy(node->right);
    free(node->category);
    free(node);
}

void abb_destroy(ABB *root) {
    node_destroy(root);
}

int abb_insert(ABB **root, const char *name) {
    if (!root || !name) return -1;
    if (!*root) {
        *root = node_create(name);
        return *root ? 0 : -1;
    }
    ABB *cur = *root;
    while (1) {
        int cmp = strcmp(name, cur->category);
        if (cmp == 0) return 0;
        if (cmp < 0) {
            if (!cur->left) { cur->left = node_create(name); return cur->left ? 0 : -1; }
            cur = cur->left;
        } else {
            if (!cur->right) { cur->right = node_create(name); return cur->right ? 0 : -1; }
            cur = cur->right;
        }
    }
}

int abb_load_categories(ABB **root, const char *path) {
    if (!root || !path) return -1;
    CSVReader *r = csv_open(path);
    if (!r) return -1;
    size_t cols = 0;
    int inserted = 0;
    int first = 1;
    while (1) {
        char **row = csv_read_row(r, &cols);
        if (!row) break;
        if (cols >= 2) {
            if (first) {
                first = 0;
                if (strcmp(row[1], "nome_categoria") == 0) { csv_free_fields(row, cols); continue; }
            }
            const char *name = row[1];
            if (name && name[0] != '\0') {
                if (abb_insert(root, name) == 0) inserted++;
            }
        }
        csv_free_fields(row, cols);
    }
    csv_close(r);
    return inserted;
}

void abb_list_categories(ABB *node) {
    if (!node) {
        printf("Nenhuma categoria disponível.\n");
        return;
    }
    abb_list_categories(node->left);
    printf("%s\n", node->category);
    abb_list_categories(node->right);
}