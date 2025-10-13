#ifndef DATA_H
#define DATA_H

#define CATEGORY_MAX 64

typedef struct {
  int id;
  char *name;
  char *filepath;
} Category;

typedef struct {
  int isbn;
  char *titulo;
  char *autor;
  int ano;
  int preco;
  int estoque;
  int vendas;
} Book;

Category *CreateCategory(int id, char *name, char *filepath);

void printCategory(Category *a);

#endif