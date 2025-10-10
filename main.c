#include <stdio.h>
#include "commands/commands.h"

int main() {
    printf("Bem vido ao sistema da biblioteca!\n");
    printCabecalho();

    int option;

    scanf("%d", &option);
    chooseOption(option);

    return 0;
}