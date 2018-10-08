#ifndef PROG_PRINCIPAIS_PROG_AUX_H
#define PROG_PRINCIPAIS_PROG_AUX_H
#define TAM_NOME 3
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* ---------- VARIAVEIS ---------- */

// Struct que armazena os dados dos programas.

typedef struct carac_progs
{
    char nome[TAM_NOME];
    int inicio;
    int duracao;
    int tipo;
} carac_Progs;

// Vetor que armazena os segundos ja ocupados.
bool vec_Segundos[60];


/* ---------- FUNÇOES ---------- */

// Retorna o tipo de programa
int analisa_buffer(char* s);

// Inicializa o vetor com nenhum espaço ocupado (falso)
void ini_Vetor(bool vec[60]);

// Armazena informações dos programas.
void preenche_Struct(carac_Progs cp);

// Insere na fila.
void insere_Fila(carac_Progs cp);

// Recebe a string e retorna o nome, inicio e duração do processo.
carac_Progs analisa_RealTime(char buf[25], int tipo);




#endif //PROG_PRINCIPAIS_PROG_AUX_H
