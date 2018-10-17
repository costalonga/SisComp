#ifndef PROG_PRINCIPAIS_PROG_AUX_H
#define PROG_PRINCIPAIS_PROG_AUX_H
#define TAM_NOME 3
#define MAX_PROGS 60
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "fila.h"
#include "listas.h"

/* ---------- VARIAVEIS ---------- */

// Struct que armazena os dados dos programas RT no vetor de RT rodando.

typedef struct carac_progs
{
    char nome[TAM_NOME];
    intmax_t inicio;
    intmax_t duracao;
    int tipo;
}carac_progs;

typedef struct pFila
{
    char nome[TAM_NOME];
    intmax_t PR;
}pFila;

/* ---------- FUNÇOES ---------- */

// Retorna o tipo de programa
int analisa_buffer(char* s);

// Inicializa o vetor com nenhum espaço ocupado (falso)
void ini_Vetor(bool vec[60], struct carac_progs v[]);

// Recebe a string e retorna o nome, inicio e duração do processo.
carac_progs analisa_RealTime(struct carac_progs v[], char buf[], int tipo, int cont);

// Recebe a struct com dados do RT e preenche a pFila para colocar na fila de prontos.
pFila pFila_RT(carac_progs st);

// Recebe a string e retorna a struct pFila com a prioridade e nome do PRIORIDADES para colocar na fila de prontos.
pFila pFila_PR(char buf[]);

// Recebe a string e retorna a struct pFila com a prioridade(8) e nome do RR para colocar na fila de prontos.
pFila pFila_RR(char buf[]);

// Usado para acessar a posição do vetor em que se encontra o programa (caso I=PX, procura por PX no vetor).
void acessa_vec_Struct(struct carac_progs v[], char s[], intmax_t* ini, int cont);

// Salva as structs no vetor de struct, para obter os dados quando necessário
void salva_no_Vetor(carac_progs st, struct carac_progs v[], int cont);

// Verifica se o RT tem tempo disponivel para ser executado.
bool verificaRT_vetorSegundos(bool v[], intmax_t ini, intmax_t fim);

// Verifica o inico e duracao do RT no vetor que contem os RT.
void verifica_vecRT(struct No* n, carac_progs c, int cont, carac_progs v[]);

// Verifica se o processo pode rodar no minuto atual
bool verifica_minAtual(double t, intmax_t ini);

//
Processo preenche_proc(Processo* p, intmax_t ini, intmax_t _fim, char nome[]);

// Recebe o programa atual rodando, verifica se deve - ou não - continuar rodando. E seleciona o próximo processo caso precise.
void troca_Contexto(double tAtual, Lista* pRodando, Lista* lEspera);


#endif //PROG_PRINCIPAIS_PROG_AUX_H
