#ifndef ABB_H
#define ABB_H

#include <stddef.h>

// Declaração da struct ABB
typedef struct ABB ABB;

// Protótipos das funções da ABB

static ABB *node_create(const char *cat,
                        const char *filename); // cria um novo nó da ABB

static void node_destroy(ABB *n); // libera a memória alocada para um nó da ABB

void abb_destroy(ABB *root); // libera a memória alocada para a abb

int abb_insert(ABB **root, const char *name,
               const char *filename); // insere uma nova categoria na abb

int abb_load_categories(
    ABB **root,
    const char *path); // carrega categorias de um arquivo CSV para a abb

void abb_list_categories(ABB *root); // lista todas as categorias presentes na
                                     // abb em ordem alfabética

int abb_is_empty(const ABB *root); //retorna 1 se a abb estiver vazia (root == NULL), 0 caso contrário

char *abb_return_file (ABB *root, const char *name);

#endif /* ABB_H */
