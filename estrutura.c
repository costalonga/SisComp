#include "estrutura.h"
#include <string.h>

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

//Retorna o indice do processo na fila
int Key_Index(Fila* fila, Prog* process) {
        
    int i=0;
    Prog* aux = fila->frente;
    
    if (aux == NULL) {
        //Se fila estiver vazia
        return -2;
    }
    
    while (aux != NULL) {
        // verifica se strings tem mesmo nome
        if (strcmp(process->nome, aux->nome) == 0) {
            return i;
        }
        aux = aux->proximo;
        i++;
    }
    
    //Retorna -1 caso nao encontre o processo procurado    
    return -1;            
}






