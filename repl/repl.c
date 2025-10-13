#include <stdio.h>
#include <stdlib.h>
#include "../structures/abb.h"
#include "repl.h"

void printCabecalho(){
    printf("\nEscolha uma das opções abaixo:\n");
    printf("\n[1] Carregar Categorias\n");
    printf("[2] Listar Categorias\n"); 
    printf("[3] Selecionar Categoria\n");
    printf("[4] Mostrar Top 1 livro mais vendido\n");
    printf("[5] Listar Top N livros mais vendidos\n");
    printf("[6] Listar registrar venda:\n");
    printf("[0] Sair\n");
}   

void chooseOption(){
    int option;
    static ABB *root = NULL;

    while(1){
        printCabecalho();
        printf("\nDigite a opção desejada: ");
        scanf("%d", &option);
        printf("\n");

        switch(option){
            case 1:
                abb_load_categories(&root, "data/categorias.csv");
                printf("Categorias carregadas com sucesso!\n");
                
                break;
            case 2:
                printf("Categorias disponíveis:\n");
                if (abb_is_empty(root)) {
                    printf("Nenhuma categoria disponível.\n");
                } else {
                    abb_list_categories(root);
                }
                break;
             case 3:
                if (abb_is_empty(root)) {
                    printf("Nenhuma categoria disponível. Carregue as categorias primeiro.\n");
                    break;
                }
                printf("Digite o nome da categoria desejada: ");
                char category[100];
                scanf("%s", category);

                if (!category || category[0] == '\0') {
                    printf("Nome de categoria inválido.\n");
                    break;
                }

                char *filename = abb_return_file(root, category);
                if (!filename) {
                    printf("Categoria não encontrada.\n");
                    break;
                }
                free(filename);
                break;
            case 4:
                //top_1_book();
                break;
            case 5:
               /* int n;
            
                printf("Digite o valor de N: ");
                scanf("%d", &n);
            
                top_n_books(n);*/

                break;
            case 6:
                //register_sale();
                break;
            case 0:
                abb_destroy(root);
                printf("Saindo...\n");
                return;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
}