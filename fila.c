#include "fila.h"
#include "prog_aux.h"
#define MAX 15

pFila prontos[MAX];
int frente = 0;
int fim = -1;
int total = 0;

/* -------------------------------- */

pFila peek() {
    return prontos[frente];
}

/* -------------------------------- */

bool vazia() {
    return total == 0;
}

/* -------------------------------- */

bool cheia() {
    return total == MAX;
}

/* -------------------------------- */

int tam() {
    return total;
}

/* -------------------------------- */

void insere_FilaProntos(pFila p) {

    if(cheia() == true) {
        printf("A fila ja esta cheia.\n");
        return;
    }

    else {

        if(fim == MAX-1) {
            fim = -1;
            //printf("Fila cheia.\n");
            //return;
        }

        fim++;
        strcpy(prontos[fim].nome, p.nome);
        prontos[fim].PR = p.PR;
        total++;
    }
}

/* -------------------------------- */

pFila remove_FilaProntos() {

    pFila primeiro;

    strcpy(primeiro.nome, prontos[frente].nome);
    primeiro.PR = prontos[frente].PR;
    frente++;

    if(frente == MAX) {
        frente = 0;
    }

    total--;
    return primeiro;
}

/* -------------------------------- */
/*
void ordena_Prioridades() {

    int i;

    for(i = 0; i < total; i++) {

        if(prontos[i].PR )
    }
}
*/

void imprime_Fila() {

    int i;

    for(i = 0; i < total; i++) {
        printf("Posicao %d da fila. Nome: %s   PR: %ld\n", i, prontos[i].nome, prontos[i].PR);
    }
}