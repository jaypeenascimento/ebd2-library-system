#ifndef STRUCTURES_HEAP_H
#define STRUCTURES_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Heap (min/max) - protótipos gerais. Implementações em `heap.c`.
 * Arquivo `heap.c` atualmente está vazio; adicione funções públicas
 * e atualize este header conforme necessário.
 */

struct Heap; /* opaco */

struct Heap *heap_create(void);
void heap_destroy(struct Heap *h);
int heap_push(struct Heap *h, void *item);
void *heap_pop(struct Heap *h);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTURES_HEAP_H */
