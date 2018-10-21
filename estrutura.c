#include "estrutura.h"
#include <string.h>

Fila* cria_Fila() {
    Fila* nova = (Fila*)malloc(sizeof(Fila));
    nova->frente = nova->fim = NULL;
    return nova;
}


void insere(Fila* fila, char* nome, int tipo, int prioridade, int inicio_RT, int duracao_RT, pid_t pid) {

    Prog* novo = (Prog*)malloc(sizeof(Prog));

    novo->nome = strdup(nome);
    novo->tipo = tipo;
    novo->prioridade = prioridade;
    novo->inicio = inicio_RT;
    novo->duracao = duracao_RT;
    novo->pid = pid;
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
    Fila* temp = fila->frente;
    fila->frente = fila->frente->proximo;
    free(temp);
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

// Retorna  1 se p1 esta antes de p2 na fila
// Retorna -1 se p2 esta antes de p1 na fila
int Compara_Index(Fila* fila, Prog* p1, Prog* p2) {
        
    int i=0;
    int indc_p1 = Key_Index(fila,p1);
    int indc_p2 = Key_Index(fila,p2);
    Prog* aux = fila->frente;
    
    if (aux == NULL) {
        //Se fila estiver vazia
        printf("Fila Vazia\n\n");
        return -2;
    }
    
    if (indc_p1 == -1) {
        printf("Erro! Processo: %s nao esta na fila", p1->nome);
        return -2;
    }
    
    if (indc_p2 == -1) {
        printf("Erro! Processo: %s nao esta na fila", p2->nome);
        return -2;
    }
    
    if (indc_p1 > indc_p2) {
        return 1;
    }
    
    else {
        return -1;
    }
}

Prog* acha_Prog_corrente(Fila* fila, pid_t pidAtual) {
    Prog* aux = fila->frente;
    
    while(aux->pid != pidAtual && aux != NULL) {
        aux = aux->proximo;
    }
    
    if(aux->pid == pidAtual) {
        return aux;
    }
    
    //Caso nao ache o PID
    return NULL;
    
}

void Fila_InsereP_existente(Fila* fila, Prog* p) {

    Prog* novo = (Prog*)malloc(sizeof(Prog));
    novo->nome = p->nome;
    novo->tipo = p->tipo;
    novo->prioridade = p->prioridade;
    novo->inicio = p->inicio;
    novo->duracao = p->duracao;
    novo->pid = p->pid;
    novo->proximo = NULL;
    if(fila->fim == NULL) {
        fila->frente = fila->fim = novo;
    }
    else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }
}

void Fila_TurnAround (Fila* fila) {
    
    Prog* oldFirst = fila->frente;
    Fila_InsereP_existente(fila, oldFirst);
    remove_primeiro(fila);
}



