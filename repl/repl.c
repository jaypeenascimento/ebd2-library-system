#include <stdio.h>
#include <stdlib.h>

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

void chooseOption(int option){
    switch(option){
        case 1:
            abb_load_categories();
            printf("Categorias carregadas com sucesso!\n");
            break;
        case 2:
            printf("Categorias disponíveis:\n");
            abb_list_categories();
            break;
        case 3:
            select_category();
            break;
        case 4:
            top_1_book();
            break;
        case 5:
            int n;
            
            printf("Digite o valor de N: ");
            scanf("%d", &n);
            
            top_n_books(n);

            break;
        case 6:
            register_sale();
            break;
        case 0:
            printf("Saindo...\n");
            exit(0);
        default:
            printf("Opção inválida. Tente novamente.\n");
    }
}