#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
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


/* Variaveis globais */



/* ***************** */



/* Funcoes */



/* ***************** */

int main() {

    int *n_linha, *f_arq, *tipo, *prioridade, *inicioRT, *duracaoRT, *trig; 
    char* nome, *prog_dep;
    int mem_n_linha, mem_f_arq, mem_tipo, mem_nome, mem_prioridade, mem_inicio, mem_duracao, mem_nome_prog_dep, mem_trig;
    time_t tInicio = time(NULL);
    unsigned int tAtual;

    
   // Fila* filaRR = cria_Fila();
    //Fila* filaPR = cria_Fila();
    Fila* filaRT = cria_Fila();


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
        if(*f_arq == 1) {
            break;
        }

        tAtual = ((unsigned int)time(NULL) - tInicio) % 60;

        if(*n_linha == 1) {
            
            if(*tipo == 1) {
                printf("Tipo: %d (RR)  Nome: %s\n", *tipo, nome);
            }


            else if(*tipo == 2) {
                printf("Tipo: %d (PR)  Nome: %s   Prioridade: %d\n", *tipo, nome, *prioridade);
            }


            else if(*tipo == 3) {

                /* Caso o programa lido dependa do termino de outro */
                if(*trig == 1) {
                    printf("Tipo: %d (RT)  Nome: %s   Inicio: %s   Duracao: %d\n", *tipo, nome, prog_dep, *duracaoRT);
                    *trig = 0;
                }

                /* Caso nao dependa de nenhum programa */
                else {
                    printf("Tipo: %d (RT)  Nome: %s   Inicio: %d   Duracao: %d\n", *tipo, nome, *inicioRT, *duracaoRT);
                    insere(filaRT, nome, *tipo, -1, *inicioRT, *duracaoRT);  
                           
                }
            }
            
            *n_linha = 0; 
        }
        

    }
    
    imprime(filaRT); 
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
