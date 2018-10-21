#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include "estrutura.h"

#define nova_linha 9020
#define fim_arq 9021
#define tipo_prog 9022  
#define nome_prog 9023
#define prioridade_prog 9024
#define inicio_rt 9025
#define duracao_rt 9026
#define nome_prog_dep 9027
#define trigger 9028

#define NUM_ELEMENTOS 10



/* Funcoes */

void executaRT(Fila* fila, unsigned int tempo);
bool checa_Criado(char* nome, char** vec);
char** aloca_Vec();
void imprime_vec(char** vec);
void insere_Processo(char** vec, char* nome);
void insere_Processo_PR(char** vec, char* nome);
int PR_Compara_Prioridade(Fila* fila, Prog* p1, Prog* p2);
void executaPR(Fila* fila, unsigned int tempo);
int Compara_Index(Fila* fila, Prog* p1, Prog* p2);
bool vazia(Fila* fila);

/* ***************** */


/* Variaveis globais */

int CORRENTE;                   /* Variavel que guarda a qtd de elementos no vetor de RT */
int CORRENTE_PR;
bool executandoRT = false;      /* Variavel que indica se ha (true) ou nao (false) um RT sendo executado */
bool executandoPR = false; 
pid_t pidAtualRT;                 /* Variavel que guarda o pid do RT sendo executado */
pid_t pidAtualPR;
char** processosRT;             /* Vetor que mantem registro dos RT ja executados */
char** processosPR;
Fila* filaEspera_PR;
bool retomada = false;          /* Variavel auxiliar que indica quando um processo foi retomado (true) ou nao (false) */

/* ***************** */

int main() {

    int *n_linha, *f_arq, *tipo, *prioridade, *inicioRT, *duracaoRT, *trig; 
    char* nome, *prog_dep;
    int mem_n_linha, mem_f_arq, mem_tipo, mem_nome, mem_prioridade, mem_inicio, mem_duracao, mem_nome_prog_dep, mem_trig;
    time_t tInicio = time(NULL);
    unsigned int tAtual;
    char* const argv[] = {NULL};

    processosRT = aloca_Vec();
    processosPR = aloca_Vec();
    CORRENTE = 0;
    CORRENTE_PR = 0;

    Fila* filaRT = cria_Fila();
    Fila* filaPR = cria_Fila();
    filaEspera_PR = cria_Fila();


    /* Atachando a memoria */

    mem_n_linha = shmget(nova_linha, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_n_linha < 0) {
        perror("Erro ao criar a memoria para salvar nLinha.\n");
        exit(1);
    }
    n_linha = (int*)shmat(mem_n_linha, 0, 0);


    mem_trig = shmget(trigger, sizeof(int), S_IWUSR | S_IRUSR);
    if(mem_trig < 0) {
        perror("Erro ao criar a memoria para salvar Trig.\n");
        exit(1);
    }
    trig = (int*)shmat(mem_trig, 0, 0);


    mem_f_arq = shmget(fim_arq, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_f_arq < 0) {
        perror("Erro ao criar a memoria para salvar fArq.\n");
        exit(1);
    }
    f_arq = (int*)shmat(mem_f_arq, 0, 0);


    mem_tipo = shmget(tipo_prog, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_tipo < 0) {
        perror("Erro ao criar a memoria para salvar Tipo.\n");
        exit(1);
    }
    tipo = (int*)shmat(mem_tipo, 0, 0);


    mem_nome_prog_dep = shmget(nome_prog_dep, sizeof(char)*3, S_IWUSR | S_IRUSR);
    if(mem_nome_prog_dep < 0) {
        perror("Erro ao criar a memoria para salvar NomeDep");
        exit(1);
    }
    prog_dep = (char*)shmat(mem_nome_prog_dep, 0, 0);


    mem_nome = shmget(nome_prog, sizeof(char*)*3, S_IRUSR | S_IWUSR);
    if(mem_nome < 0) {
        perror("Erro ao criar a memoria para salvar Nome.\n");
        exit(1);
    }
    nome = (char*)shmat(mem_nome, 0, 0);


    mem_prioridade = shmget(prioridade_prog, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_prioridade < 0) {
        perror("Erro ao criar a memoria para salvar Prioridade.\n");
        exit(1);
    }
    prioridade = (int*)shmat(mem_prioridade, 0, 0);


    mem_inicio = shmget(inicio_rt, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_inicio < 0) {
        perror("Erro ao criar a memoria para salvar Inicio.\n");
        exit(1);
    }
    inicioRT = (int*)shmat(mem_inicio, 0, 0);


    mem_duracao = shmget(duracao_rt, sizeof(int), S_IRUSR | S_IWUSR);
    if(mem_duracao < 0) {
        perror("Erro ao criar a memoria para salvar Duracao.\n");
        exit(1);
    }
    duracaoRT = (int*)shmat(mem_duracao, 0, 0);


    /* Inicio do escalonamento */


    while(1) {
        
        /* Condicao de parada. Nenhuma linha nova no arquivo e nenhum programa para executar */
        if(*f_arq == 1 && filaRT == NULL) {
            break;
        }

        tAtual = ((unsigned int)time(NULL) - tInicio) % 60;

        if(tAtual > 60) {
            tAtual = 0;
        }
        
        if(*n_linha == 1) {
            
            if(*tipo == 1) {
                printf("Tipo: %d (RR)  Nome: %s\n", *tipo, nome);
            }


            else if(*tipo == 2) {
                //printf("Tipo: %d (PR)  Nome: %s   Prioridade: %d\n", *tipo, nome, *prioridade);
                insere(filaPR, nome, *tipo, *prioridade, -1, -1);
            }


            else if(*tipo == 3) {

                /* Caso o programa lido dependa do termino de outro */
                if(*trig == 1) {
                    printf("Tipo: %d (RT)  Nome: %s   Inicio: %s   Duracao: %d\n", *tipo, nome, prog_dep, *duracaoRT);
                    *trig = 0;
                }

                /* Caso nao dependa de nenhum programa */
                else {
                    insere(filaRT, nome, *tipo, -1, *inicioRT, *duracaoRT);      
                }
            }

            *n_linha = 0; 
        }
        
            executaRT(filaRT, tAtual);
            /* Alterando a partir dauqi */
            executaPR(filaPR, tAtual);  
        }
    }
    
    /* Terminou escalonamento */


    /* Liberando memoria */
    
    shmdt(n_linha);
    shmdt(tipo);
    shmdt(f_arq);
    shmdt(prioridade);
    shmdt(inicioRT);
    shmdt(duracaoRT);
    shmdt(nome);
    shmdt(trig);
    shmdt(prog_dep);

    shmctl(mem_nome_prog_dep, IPC_RMID, 0);
    shmctl(mem_trig, IPC_RMID, 0);
    shmctl(mem_n_linha, IPC_RMID, 0);
    shmctl(mem_tipo, IPC_RMID, 0);
    shmctl(mem_f_arq, IPC_RMID, 0);
    shmctl(mem_prioridade, IPC_RMID, 0);
    shmctl(mem_inicio, IPC_RMID, 0);
    shmctl(mem_duracao, IPC_RMID, 0);
    shmctl(mem_nome, IPC_RMID, 0);
    

    return 0;
}

/* Funcao que trata do escalonamento dos processos RT */
void executaRT(Fila* fila, unsigned int tempo) {

    Prog* aux = fila->frente;
    char* const argv[] = {NULL};
    pid_t pid;
    bool verifica_Executado = false;


    if(executandoRT == false) {
    
        while(tempo != aux->inicio) {
            aux = aux->proximo;

            if(aux->proximo == NULL) {
                return;
            }
        }

        verifica_Executado = checa_Criado(aux->nome, processosRT);
        insere_Processo(processosRT, aux->nome);

        if(verifica_Executado == true) {

            if(retomada == false) {
                    printf("Retomando o processo de ID %d\n", aux->pid);
                    kill(aux->pid, SIGCONT);
                    executandoRT = true;
                    pidAtualRT = aux->pid;
                    retomada = true;
                    return;
                }
        }


        if((pid = fork()) == 0) {
                execv(aux->nome, argv);
            }

        else {
            retomada = false;
            executandoRT = true;
            printf("ID do processo criado aos %d segundos: %d\n", tempo, pid);
            aux->pid = pid;
            pidAtualRT = pid;
        }
    }

    else {
        aux = acha_Prog_corrente(fila, pidAtualRT);
        if(tempo == (aux->duracao+aux->inicio)) {
            kill(aux->pid, SIGSTOP);
            printf("Processo de ID %d parado aos %d segundos.\n", aux->pid, tempo);
            executandoRT = false;
            retomada = false;
        }

    }

}

/* Funcao que verifica se o processo ja foi criado e executado previamente. Utilizada a partir do segundo minuto. */
bool checa_Criado(char* nome, char** vec) {

    int i = 0;

    for(i = 0; i < CORRENTE; i++) {
        if(strcmp(nome, vec[i]) == 0) {
            
            return true;
        }
    }

    return false;
}

/* Funcao auxiliar que serve para alocar o vetor de RT */
char** aloca_Vec() {

    char** vec;
    vec = (char**)malloc(sizeof(char)*NUM_ELEMENTOS);

    int i;
    for(i = 0; i < NUM_ELEMENTOS; i++) {
        vec[i] = (char*)malloc(sizeof(char)*5);
    }

    return vec;
}

/* Funcao auxiliar que imprime o vetor de RT (verificacao) */
void imprime_vec(char** vec) {
    int i;

    for(i = 0; i < CORRENTE; i++) {
        printf("Processo %s na posicao %d.\n", vec[i], i);
    }
}

/* Funcao que insere um processo no vetor de RT para manter registro dos programas executados */
void insere_Processo(char** vec, char* nome) {

    int i;

    for(i = 0; i < CORRENTE; i++) {
        if(strcmp(nome, vec[i]) == 0) {
            return;
        }
    }

    vec[CORRENTE] = strdup(nome);
    CORRENTE++;
}

void insere_Processo_PR(char** vec, char* nome) {

    int i;

    for(i = 0; i < CORRENTE_PR; i++) {
        if(strcmp(nome, vec[i]) == 0) {
            return;
        }
    }

    vec[CORRENTE_PR] = strdup(nome);
    CORRENTE_PR++;
}


/*
* Retorna 1 se p1 eh mais prioritario que p2
* Retorna -1 se p1 eh menos prioritario que p1
* Casos de prioridade iguais, desempate eh feito pelo tempo de chegada (posicao na fila)
*/
int PR_Compara_Prioridade(Fila* fila, Prog* p1, Prog* p2) {
    
    int prio1 = p1->prioridade;
    int prio2 = p2->prioridade;
    
    if (prio1 > prio2) {
        return 1;
    }

    else if (prio1 == prio2) {
        //Considerando que 2 processos nunca vao chegar ao mesmo tempo
        return Compara_Index(fila, p1, p2);       
    }
    
    else {
        return -1;
    }    
}


void executaPR(Fila* fila, unsigned int tempo) {
    
    Prog* aux = fila->frente;
    char* const argv[] = {NULL};
    pid_t pid;

    //obtendo o primeiro processo PR mais prioritario na fila
    while(aux->tipo != 2) {
        aux = aux->proximo;

        if(aux->proximo == NULL) {
            return;
        }
    }
    
    //Se nao houver nenhum processo RT ou de PR executando no momento ele pode ser executado
    if (executandoRT == false && executandoPR == false) {
         
         // Se nao tiver programa na fila de espera
        if(vazia(filaEspera_PR) == true) {
            if((pid = fork()) == 0) {
                execv(aux->nome, argv);
            }

            else {
                executandoPR = true;
                printf("ID do processo criado aos %d segundos: %d\n", tempo, pid);
                aux->pid = pid;
                pidAtualPR = pid; 
                insere_Processo_PR(processosPR, aux->nome);
            }
        }

        // Verifica prioridade e executa de fila, se for maior
        else {
            int comparacao2 = PR_Compara_Prioridade(filaEspera_PR, aux, filaEspera_PR->frente);

            /* Executa o novo, nao o da fila de espera*/
            if(comparacao2 == 1) {
                printf("Retomando o processo de ID: %d", aux->pid);
                kill(aux->pid, SIGCONT);
                executandoPR = true;
                pidAtualPR = aux->pid;
                return;
            }

            /* Executa da fila de espera */
            else {
                printf("Retomando o processo de ID: %d", filaEspera_PR->frente->pid);
                kill(filaEspera_PR->frente->pid, SIGCONT);
                executandoPR = true;
                pidAtualPR = filaEspera_PR->frente->pid;
                return;
            }
        }
    }
    
    //verifica se é um de prioridade maior ou menor em execucao
    else if (executandoPR == true) {
        
        //Pega processo PR atual corrente
        Prog* PR_AtualProcess = acha_Prog_corrente(fila, pidAtualPR);     // <<<<<<<<<<<<<<---------------------
        
        //Compara processo atual sendo executado com o processo PR encontrado na fila
        int comparacao = PR_Compara_Prioridade(fila, aux, PR_AtualProcess);
        
        //Se aux eh mais prioritario que processo atual
        if (comparacao == 1) { 
            
            if((pid = fork()) == 0) {
                execv(aux->nome, argv);
            }

            else {
                kill(PR_AtualProcess->pid, SIGSTOP);
                executandoPR = true;
                printf("ID do processo (mais prioritario) criado aos %d segundos: %d\n", tempo, pid);
                aux->pid = pid;
                pidAtualPR = pid;      
                insere(filaEspera_PR, aux->nome, aux->tipo, aux->prioridade, -1, -1);
            }
        }
        
        else {
            
        }
    }
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


bool vazia(Fila* fila) {

    if(fila->frente == NULL) {
        return true;
    }

    return false;
}