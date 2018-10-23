#ifndef T1_ESTRUTURA_H
#define T1_ESTRUTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct programa Prog;
typedef struct fila Fila;

struct programa
{
    char* nome;
    int tipo;
    int prioridade;
    int inicio;
    int duracao;
    bool executado;
    pid_t pid;
    Prog* proximo;
};

struct fila 
{
    Prog* frente;
    Prog* fim;
};

/* Cria a fila que armazena os dados dos programas */
Fila* cria_Fila();

/* Insere um elemento na lista dada */
void insere(Fila* fila, char* nome, int tipo, int prioridade, int inicio_RT, int duracao_RT, pid_t pid);

/* Imprime a lista */
void imprime(Fila* fila);

/* Remove primeiro elemento da fila */
void remove_primeiro(Fila* fila);

// Dado um processo retorna sua posicao na fila 
// Retorna -1 caso nao ache o elemento e -2 caso fila esteja vaiza
int Key_Index(Fila* fila, Prog* process);

// Retorna  1 se p1 esta antes de p2 na fila
// Retorna -1 se p2 esta antes de p1 na fila
int Compara_Index(Fila* fila, Prog* p1, Prog* p2);

Prog* acha_Prog_corrente(Fila* fila, pid_t pidAtual);

void Fila_InsereP_existente(Fila* fila, Prog* p);

void Fila_TurnAround (Fila* fila);

#endif //ESTRUTURA_H
