#include "fila.h"
#include "prog_aux.h"


struct No* cria_No(char nome[], intmax_t PR) {

    struct No* temp = (struct No*)malloc(sizeof(struct No));
    strcpy(temp->nome, nome);
    temp->PR = PR;
}

/* -------------------------------- */

struct Fila* cria_Fila() {
    struct Fila* f = (struct Fila*)malloc(sizeof(struct Fila));
    f->frente = f-> fim = NULL;
    return f;
}

/* -------------------------------- */

void insere_FilaProntos(struct Fila* f, char nome[], intmax_t PR) {

    struct No* temp = cria_No(nome, PR);

    if(f->fim == NULL)
    {
        f->frente = f->fim = temp;
        return;
    }

    f->fim->prox = temp;
    f->fim = temp;
    
}

/* -------------------------------- */

struct No* remove_FilaProntos(struct Fila* f) {

    if(f->frente == NULL) {
        return NULL;
    }

    struct No* temp = f->frente;
    f->frente = f->frente->prox;

    if(f->frente == NULL) {
        f->fim = NULL;
    }

    return temp;
}

/* -------------------------------- */

void ordena_Prioridades(struct Fila* f) {

    struct No* ptr1;
    struct No* lptr = NULL;
    bool troca;
    //bool ord;

    if(f == NULL)
        return;
    
    do
    {
        troca = false;
        ptr1 = f->frente;

        while(ptr1->prox != lptr) {

            if(ptr1 -> PR > ptr1->prox->PR) {
                intmax_t temp1 = ptr1->PR;
                char temp2[TAM_NOME];
                strcpy(temp2, ptr1->nome);

                strcpy(ptr1->nome, ptr1->prox->nome);
                ptr1->PR = ptr1->prox->PR;
                strcpy(ptr1->prox->nome, temp2);
                ptr1->prox->PR = temp1;
                troca = true;
            }

            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    }
    while(troca);
}

/* -------------------------------- */

void imprime_Fila(struct Fila* f) {

    struct No* temp;
    temp = f->frente;
    int i = 0;

    while(temp != NULL) {
        printf("Posicao %d ->>> Nome: %s   Prioridade: %ld\n", i, temp->nome, temp->PR);
        temp = temp->prox;
        i++;
    }

}