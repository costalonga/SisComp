#include "estrutura.h"

Fila* cria_Fila() {
    Fila* nova = (Fila*)malloc(sizeof(Fila));
    nova->frente = nova->fim = NULL;
    return nova;
}


void insere(Fila* fila, char* nome, int tipo, int prioridade, int inicio_RT, int duracao_RT) {

    Prog* novo = (Prog*)malloc(sizeof(Prog));

    novo->nome = strdup(nome);
    novo->tipo = tipo;
    novo->prioridade = prioridade;
    novo->inicio = inicio_RT;
    novo->duracao = duracao_RT;
    pid_t pid = 0;
    novo->proximo = NULL;

    
    if(fila->fim == NULL) {
        fila->frente = fila->fim = novo;
    }
    else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }

    
}


void imprime(Fila* fila) {

    Prog* aux = (Prog*)malloc(sizeof(Prog));
    aux = fila->frente;

    if(fila->frente == NULL) {
        printf("Lista vazia.\n");
    }

    while(aux != NULL) {
        printf("Nome: %s  Inicio: %d  Duracao: %d\n", aux->nome, aux->inicio, aux->duracao);
        aux = aux->proximo;
    }
}


void remove_primeiro(Fila* fila) {

    fila->frente = fila->frente->proximo;
    free(fila->frente);
}






