#ifndef PROG_PRINCIPAIS_FILA_H
#define PROG_PRINCIPAIS_FILA_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define TAM_NOME 3

// Struct que armazena o nome do programa e a prioridade na fila de PRONTOS.
struct No
{
    char nome[TAM_NOME];
    intmax_t PR;
    struct No* prox;
};

struct Fila
{
    struct No* frente;
    struct No* fim;
};


// Cria um nó com nome e prioridade para inserir na fila.
struct No* cria_No(char nome[], intmax_t PR);

// Cria uma fila (lista encadeada).
struct Fila* cria_Fila();

// Insere um programa (Nó) na fila.
void insere_FilaProntos(struct Fila* f, char nome[], intmax_t PR);

// Remove o programa (Nó) da primeira posição.
struct No* remove_FilaProntos();

// Ordena a fila de acordo com as prioridades dos programas.
void ordena_Prioridades(struct Fila* f);

// Imprime a fila com o nome e prioridade dos programas (prontos).
void imprime_Fila(struct Fila* f);

#endif //PROG_PRINCIPAIS_FILA_H