#ifndef CSVREADER_H
#define CSVREADER_H

#include <stddef.h>
//protótipos da struct e funções do leitor CSV

typedef struct CSVReader CSVReader; //declaração da struct CSVReader

CSVReader *csv_open(const char *path); //abre um arquivo CSV para leitura.

char **csv_read_row(CSVReader *r, size_t *out_count); //lê uma linha do arquivo CSV e retorna um array de strings.

void csv_free_fields(char **fields, size_t count); //libera a memória alocada para os campos lidos.

void csv_close(CSVReader *r); //fecha o leitor e libera recursos internos.

char **csv_load_first_column(const char *path, size_t *out_count); //carrega a primeira coluna do arquivo CSV e retorna um array de strings.

#endif /* CSVREADER_H */