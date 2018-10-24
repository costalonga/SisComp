#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
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
#define esperaRR 
#define EVER ;;


/* Funcoes */

void executaRT(Fila* fila, unsigned int tempo, Fila* filaPR);
void executaRR(Fila* fila, unsigned int tempo);
bool checa_Criado(char* nome, char** vec);
char** aloca_Vec();
void imprime_vec(char** vec);
void insere_Processo(char** vec, char* nome);
bool verifica_FilaDeEspera(Fila* fila, char* nome);
int PR_Compara_Prioridade(Prog* p1, Prog* p2);      
bool executaPR(Fila* fila, unsigned int tempo);     
bool vazia(Fila* fila);
bool compara_Prioridade(Fila* fila, int prio);
int encontra_Dependente(Fila* fila, char* nome);

/* ***************** */


/* Variaveis globais */

int CORRENTE;                   /* Variavel que guarda a qtd de elementos no vetor de RT */
bool executandoRT = false;      /* Variavel que indica se ha (true) ou nao (false) um RT sendo executado */
bool executandoPR = false;      /* Variavel que indica se ha (true) ou nao (false) um PR sendo executado */
pid_t pidAtualRT;               /* Variavel que guarda o pid do RT sendo executado */
pid_t pidAtualPR;               /* Variavel que guarda o pid do PR sendo executado */
char** processosRT;             /* Vetor que mantem registro dos RT ja executados */
Fila* filaEspera_PR;            /* Fila de espera dos processos de prioridade */
bool retomada = false;          /* Variavel auxiliar que indica quando um processo foi retomado (true) ou nao (false) */
bool minutoNOVO;                /* Variavel que indica se o minuto recomecou */

/* ***************** */

int main() {

    int *n_linha, *f_arq, *tipo, *prioridade, *inicioRT, *duracaoRT, *trig; 
    char* nome, *prog_dep;
    int mem_n_linha, mem_f_arq, mem_tipo, mem_nome, mem_prioridade, mem_inicio, mem_duracao, mem_nome_prog_dep, mem_trig;
    time_t tInicio = time(NULL);
    unsigned int tAtual;
    char* const argv[] = {NULL};

    bool teste;

    processosRT = aloca_Vec();
    CORRENTE = 0;

    Fila* filaRR = cria_Fila();
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
        
        tAtual = ((unsigned int)time(NULL) - tInicio) % 60;

        if(tAtual > 60) {
            tAtual = 0;
            minutoNOVO = true;
        }

        if(*n_linha == 1) {
            
            if(*tipo == 1) {
                insere(filaRR, nome, *tipo, -1, -1, -1, -1);
            }


            else if(*tipo == 2) {
                insere(filaPR, nome, *tipo, *prioridade, -1, -1, -1);
            }


            else if(*tipo == 3) {

                /* Caso o programa lido dependa do termino de outro */
                if(*trig == 1) {
                    int ini_dependente = encontra_Dependente(filaRT, prog_dep);
                    if(ini_dependente != -1) {
                        if(ini_dependente + *duracaoRT < 60) {
                            insere(filaRT, nome, *tipo, -1, ini_dependente, *duracaoRT, -1);
                        }
                    }
                    *trig = 0;
                }

                /* Caso nao dependa de nenhum programa */
                else {
                    if((*inicioRT + *duracaoRT) < 60) {
                        insere(filaRT, nome, *tipo, -1, *inicioRT, *duracaoRT, -1);
                    }      
                }
            }

            *n_linha = 0; 
        }

            executaRT(filaRT, tAtual, filaPR);

            if(executandoRT == false) {
                
                if(executaPR(filaPR, tAtual) == true){
                    Fila_TurnAround(filaPR);
                }
                
                if(executandoRT == false && executandoPR == false) {
                    
                    executaRR(filaRR, tAtual);
                
            } 
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
void executaRT(Fila* fila, unsigned int tempo, Fila* filaPR) {

    Prog* aux = fila->frente;

    if(aux == NULL) {
        return;
    }

    char* const argv[] = {NULL};
    pid_t pid;
    bool verifica_Executado = false;

    if(executandoRT == false) {
    
        while(aux->inicio != tempo) {
            aux = aux->proximo;
            
            if(aux == NULL) {
                return;
            }
        }

        verifica_Executado = checa_Criado(aux->nome, processosRT);
        insere_Processo(processosRT, aux->nome);

        if(verifica_Executado == true) {

            if(retomada == false) {

                if(executandoPR == true) {
                    printf("1 Interrompendo processo de prioridades de ID %d aos %d segundos\n", pidAtualPR, tempo);
                    Prog* prATUAL = acha_Prog_corrente(filaPR, pidAtualPR);
                    kill(prATUAL->pid, SIGSTOP);
                    Fila_InsereP_existente(filaEspera_PR, prATUAL);
                    executandoPR = false;
                }
            
                printf("1 Retomando o processo de ID %d\n", aux->pid); ///////////////
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

            if(executandoPR == true) {
                printf("2 Interrompendo processo de prioridades de ID %d aos %d segundos\n", pidAtualPR, tempo);
                Prog* prATUAL = acha_Prog_corrente(filaPR, pidAtualPR);
                kill(prATUAL->pid, SIGSTOP);
                Fila_InsereP_existente(filaEspera_PR, prATUAL);
                executandoPR = false;
            }

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

/* 1 se prioridade de p1 > prioridade de p2, -1 caso contrario */
int PR_Compara_Prioridade(Prog* p1, Prog* p2) {    
    
    int prio1 = p1->prioridade;
    int prio2 = p2->prioridade;
    
    if (prio1 > prio2) {
        return 1;
    }

    else {
        return -1;
    }    
}

/* Trata do escalonamento dos processos de prioridade */
bool executaPR(Fila* fila, unsigned int tempo) {
    
    bool filaPR_Vazia = vazia(fila);

    if(filaPR_Vazia == true) {
        return false;
    }

    if(minutoNOVO == true) {
        fila = filaEspera_PR;
    }

    Prog* aux = fila->frente;
    char* const argv[] = {NULL};
    pid_t pid;
    bool espera_Vazia = false;
    int maior_Prioridade;

    espera_Vazia = vazia(filaEspera_PR);

    
    // Nao tem nenhum PR executando
    if(executandoPR == false) {

        if(espera_Vazia == true) {
            if((pid = fork()) == 0) {
                execv(aux->nome, argv);
            }

            else {
                printf("1-Executando programa de ID %d aos %d segundos\n", pid, tempo);
                aux->pid = pid;
                aux->executado = true;
                executandoPR = true;
                pidAtualPR = pid;  
                return true;
            }
        }

        // Executa da fila
        else {
            Prog* primeiro_FilaEspera = filaEspera_PR->frente;

                if(primeiro_FilaEspera->executado == true) {
                    printf("2 Retomando processo de ID %d aos %d segundos \n", primeiro_FilaEspera->pid, tempo);
                    kill(primeiro_FilaEspera->pid, SIGCONT);    
                    executandoPR = true;
                    pidAtualPR = primeiro_FilaEspera->pid;
                    remove_primeiro(filaEspera_PR);
                    return true;
                }

                else {
                    if((pid = fork()) == 0) {
                        execv(primeiro_FilaEspera->nome, argv);
                    }

                    else {
                        printf("2-Executando processo de ID %d aos %d segundos\n", pid, tempo);
                        executandoPR = true;
                        pidAtualPR = pid;
                        primeiro_FilaEspera->pid = pid;
                        primeiro_FilaEspera->executado = true;
                        remove_primeiro(filaEspera_PR);
                        return true;
                    }
                }
        }
    }

    // Ja tem algum PR executando, compara prioridade
    else {
        bool prio;

        /*
        if(aux->pid == pidAtualPR) {
            return false;
        }   
        */

        Prog* corr = acha_Prog_corrente(fila, pidAtualPR);

        // Comparando ATUAL com NOVO
        prio = compara_Prioridade(fila, corr->prioridade);

        // Se o atual for MAIS prioritario  
        if(prio == false) {
                Fila_InsereP_existente(filaEspera_PR, aux);
                return false;
        }

        // Se o novo for MAIS prioritario
        else {
            printf("1 Processo de ID %d interrompido aos %d segundos\n", pidAtualPR, tempo);
            kill(pidAtualPR, SIGSTOP);
            Fila_InsereP_existente(filaEspera_PR, corr);    

            if(aux->executado == false){
                if((pid = fork()) == 0) {
                    execv(aux->nome, argv);
                }

                else {
                    printf("Executando processo de ID %d aos %d segundos\n", pid, tempo);
                    aux->executado = true;
                    aux->pid = pid;
                    executandoPR = true;
                    pidAtualPR = pid;
                    return true;
                }
            }

            else {
                kill(aux->pid, SIGCONT);
                pidAtualPR = aux->pid;
                return true;
            }
        }
    }
}

/* Compara prioridade dos processos*/
bool compara_Prioridade(Fila* fila, int prio) {

    Prog* aux = fila->frente;
    /*
    while(aux->pid != pidAtualPR) {
        aux = aux->proximo;
    }
    */

    // Se o processo recebido tiver uma prioridade MENOR que o processod e PID Atual return true 
    //(prio eh mais prioritario que o processo atual) -> fila de espera (atual)
    if(aux->prioridade > prio) {
        return false;
    }

    // Se o atual for MAIS prioritario -> fila de espera (novo)
    else {
        return true;
    }
}

/* Verifica se a fila dada esta vazia*/
bool vazia(Fila* fila) {

    if(fila->frente == NULL) {
        return true;
    }

    return false;
}

/* Verifica se um processo esta na fila de espera*/
bool verifica_FilaDeEspera(Fila* fila, char* nome) {

    Prog* aux = fila->frente;

    while(aux != NULL) {
        if(strcmp(aux->nome, nome) == 0) {
            return true;
        }

        aux = aux->proximo;
    }

    return false;
}

/* Trata do escalonamento de Round Robins*/
void executaRR(Fila* fila, unsigned int tempo) {

    pid_t pid;
    bool fila_Vazia;
    double parada = (double) tempo + 0.5;
    char* argv[] = {NULL};

    fila_Vazia = vazia(fila);
  
    if(fila_Vazia == false) {
        Prog* RR = fila->frente;

        if((pid = fork()) == 0) {
            execv(RR->nome, argv);
        }

        else {  
            printf("Executando programa (RR) de ID %d aos %d segundos\n", pid, tempo);
            sleep(1);
            kill(pid, SIGKILL);           
            printf("Programa de ID %d terminado aos %f segundos\n", pid, parada);
            remove_primeiro(fila);
            return;
        }
    }

    else {
        return;
    }
}

/* Para casos em que os Real Time dependem do termino de outro */
int encontra_Dependente(Fila* fila, char* nome) {

    Prog* aux = fila->frente;

    while(strcmp(nome, aux->nome) != 0) {
        aux = aux->proximo;
        if(aux == NULL) {
            return -1;
        }
    }

    return (aux->inicio + aux->duracao) + 1;
}