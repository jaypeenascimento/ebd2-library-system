#ifndef ABB_H
#define ABB_H

#include <stddef.h>

// Declaração da struct ABB
typedef struct ABB ABB;

// Protótipos das funções da ABB

static ABB *node_create(const char *cat); //cria um novo nó da ABB

static void node_destroy(ABB *n); //libera a memória alocada para um nó da ABB

void abb_destroy(ABB *t); //libera a memória alocada para a abb

int abb_insert(ABB **root, const char *name); //insere uma nova categoria na abb

int abb_load_categories(ABB **t, const char *path); //carrega categorias de um arquivo CSV para a abb

void abb_list_categories(ABB *t); //lista todas as categorias presentes na abb em ordem alfabética

char *abb_return_file (ABB *t, const char *name);

#endif /* ABB_H */