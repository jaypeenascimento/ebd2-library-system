#ifndef STRUCTURES_ABB_H
#define STRUCTURES_ABB_H

#ifdef __cplusplus
extern "C" {
#endif

/* ABB header (existe um `structures/abb.h` já; manter) */

struct Abb;

struct Abb *abb_create(void);
void abb_destroy(struct Abb *a);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTURES_ABB_H */
