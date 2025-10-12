#ifndef COMMANDS_H
#define COMMANDS_H

#include "../data.h"
#include "../structures/heap.h"

void load_categories(); // função para carregar categorias

void list_categories(); // função para listar categorias

void load_books(Heap **heap,
                char *filename); // função para selecionar categoria

Book *top_1_book(Heap **heap); // função para mostrar o top 1 livro mais vendido

void top_n_books(int n); // função para mostrar o top N livros mais vendidos

void register_sale(); // função para registrar uma venda

#endif /* COMMANDS_H */
