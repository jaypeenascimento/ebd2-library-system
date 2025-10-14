#include "data.h"

#include <stdio.h>
#include <stdlib.h>

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
  if (cat == NULL) {
    printf("Couldn't create Category: Failed to allocate memory!");
  }

  cat->id = id;
  cat->name = name;
  cat->filepath = filepath;

  return cat;
}

Book *CreateBook(int isbn, char *titulo, char *autor, int ano, int preco,
                 int estoque, int vendas) {
  Book *b = malloc(sizeof(Book));
  if (b == NULL) {
    printf("Couldn't create book: Failed to allocate memory!");
  }

  b->isbn = isbn;
  b->titulo = titulo;
  b->autor = autor;
  b->ano = ano;
  b->preco = preco;
  b->estoque = estoque;
  b->vendas = vendas;

  return b;
}

void printBook(Book *b) {
  if (b == NULL) {
    printf("Tried to print a NULL book!\n");
    return;
  }

  printf("\n");
  printf("\"%s\" - %s, %d. (ISBN: %d)\n", b->titulo, b->autor, b->ano, b->isbn);
  printf("R$ %d | ", b->preco);
  printf("%d unidades restantes | ", b->estoque);
  printf("%d unidades vendidas\n", b->vendas);
}
