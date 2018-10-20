#ifndef T1_ESTRUTURA_H
#define T1_ESTRUTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct programa Prog;
typedef struct fila Fila;

struct programa
{
    char* nome;
    int tipo;
    int prioridade;
    int inicio;
    int duracao;
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
void insere(Fila* fila, char* nome, int tipo, int prioridade, int inicio_RT, int duracao_RT);

/* Imprime a lista */
void imprime(Fila* fila);

/**/
void remove_primeiro(Fila* fila);


#endif //ESTRUTURA_H