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
    printf("Book: \n");
    printf("NULL Book!\n");
    return;
  }

  printf("\n");
  printf("Isbn: %d\n", b->isbn);
  printf("Titulo: %s\n", b->titulo);
  printf("Autor: %s\n", b->autor);
  printf("Ano: %d\n", b->ano);
  printf("Preco: %d\n", b->preco);
  printf("Estoque: %d\n", b->estoque);
  printf("Vendas: %d\n", b->vendas);

  printf("\n");
}
