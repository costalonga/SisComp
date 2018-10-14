#ifndef PROG_PRINCIPAIS_PROG_AUX_H
#define PROG_PRINCIPAIS_PROG_AUX_H
#define TAM_NOME 3
#define MAX_PROGS 60
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

/* ---------- VARIAVEIS ---------- */

// Struct que armazena os dados dos programas RT no vetor de RT rodando.

typedef struct carac_progs
{
    char nome[TAM_NOME];
    intmax_t inicio;
    intmax_t duracao;
    int tipo;
}carac_progs;


/* ---------- FUNÇOES ---------- */

// Retorna o tipo de programa
int analisa_buffer(char* s);

// Inicializa o vetor com nenhum espaço ocupado (falso)
void ini_Vetor(bool vec[60], struct carac_progs v[]);

// Recebe a string e retorna o nome, inicio e duração do processo.
carac_progs analisa_RealTime(struct carac_progs v[], char buf[], int tipo, int cont);

// Usado para acessar a posição do vetor em que se encontra o programa (caso I=PX, procura por PX no vetor).
void acessa_vec_Struct(struct carac_progs v[], char s[], intmax_t* ini, int cont);

// Salva as structs no vetor de struct, para obter os dados quando necessário
void salva_no_Vetor(carac_progs st, struct carac_progs v[], int cont);




#endif //PROG_PRINCIPAIS_PROG_AUX_H
