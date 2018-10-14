#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Struct que armazena o nome do programa e a prioridade na fila de PRONTOS.
typedef struct prog_Fila
{
    char nome[TAM_NOME];
    int PR;
    int arrival; //tempo para criterio de desempate
} pFila;



// Verifica primeiro prog. da fila de prontos (nome e prioridade)
pFila peek();

// Verifica se a fila está vazia.
bool vazia();

// Verifica se a fila está cheia.
bool cheia();

// Verifica quantos programas estão na fila (opcional).
int tam();

// Insere um programa na ultima posição da fila.
void insere_FilaProntos(struct pFila);

// Remove o programa da primeira posição.
pFila remove_FilaProntos();

// Ordena a fila de acordo com as prioridades dos programas.
void ordena_Prioridades();
