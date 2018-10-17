#ifndef PROG_PRINCIPAIS_PROCESSOS_H
#define PROG_PRINCIPAIS_PROCESSOS_H

#include <stdio.h>
#include <stdlib.h> 

typedef enum estado{
	novo,
	pronto,
	esperando,
	executando,
	terminado
} Estado;

typedef enum tipoProc {
    RT, //real time
    PR, //priority
    RR //round robin
} TipoProc;

typedef struct Processo{
    char nome[3];
    int ini;
    int fim;
	Estado estado;
	int pid;
} Processo;
#endif //PROG_PRINCIPAIS_LISTAS_H