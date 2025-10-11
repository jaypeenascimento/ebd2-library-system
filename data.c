#include "data.h"

#include <stdio.h>
#include <stdlib.h>

// Helpers for debugging
void printCategory(Category *a) {
  if (a == NULL) {
    printf("Category: \n");
    printf("NULL CATEGORY!\n");

    return;
  }

  printf("\n");

  printf("Category: \n");
  printf("Id: %d\n", a->id);
  printf("Name: %s\n", a->name);
  printf("Filepath: %s\n", a->filepath);

  printf("\n");
}

Category *CreateCategory(int id, char *name, char *filepath) {
  Category *cat = malloc(sizeof(Category));

  cat->id = id;
  cat->name = name;
  cat->filepath = filepath;

  return cat;
}
