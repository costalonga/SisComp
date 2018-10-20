#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#define nova_linha 9020
#define fim_arq 9021
#define tipo_prog 9022  
#define nome_prog 9023
#define prioridade_prog 9024
#define inicio 9025
#define duracao 9026
#define nome_prog_dep 9027
#define trigger 9028

int main() {

    FILE* exec;
    pid_t escalonador_PID;
    int i, j, k, espacos;
    int *n_linha, *f_arq, *tipo, *prioridade, *inicioRT, *duracaoRT, *trig; 
    int mem_n_linha, mem_f_arq, mem_tipo, mem_nome, mem_prioridade, mem_inicio, mem_duracao, mem_nome_prog_dep, mem_trig;
    char* linha_Lida = (char*)malloc(sizeof(char)*25);
    char* nome, *prog_dep;
    char* const argv[] = {NULL};
    

    /* Criando e atachando a memoria */

    mem_trig = shmget(trigger, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_trig < 0) {
        perror("Erro ao criar a memoria para salvar Trig.\n");
        exit(1);
    }
    trig = (int*)shmat(mem_trig, 0, 0);
    

    mem_n_linha = shmget(nova_linha, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_n_linha < 0) {
        perror("Erro ao criar a memoria para salvar nLinha.\n");
        exit(1);
    }
    n_linha = (int*)shmat(mem_n_linha, 0, 0);


    mem_f_arq = shmget(fim_arq, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_f_arq < 0) {
        perror("Erro ao criar a memoria para salvar fArq.\n");
        exit(1);
    }
    f_arq = (int*)shmat(mem_f_arq, 0, 0);


    mem_tipo = shmget(tipo_prog, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_tipo < 0) {
        perror("Erro ao criar a memoria para salvar Tipo.\n");
        exit(1);
    }
    tipo = (int*)shmat(mem_tipo, 0, 0);


    mem_nome = shmget(nome_prog, sizeof(char*)*3, IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_nome < 0) {
        perror("Erro ao criar a memoria para salvar Nome.\n");
        exit(1);
    }
    nome = (char*)shmat(mem_nome, 0, 0);
    

    mem_nome_prog_dep = shmget(nome_prog_dep, sizeof(char)*3, IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_nome_prog_dep < 0) {
        perror("Erro ao criar a memoria para salvar NomeDep");
        exit(1);
    }
    prog_dep = (char*)shmat(mem_nome_prog_dep, 0, 0);


    mem_prioridade = shmget(prioridade_prog, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_prioridade < 0) {
        perror("Erro ao criar a memoria para salvar Prioridade.\n");
        exit(1);
    }
    prioridade = (int*)shmat(mem_prioridade, 0, 0);


    mem_inicio = shmget(inicio, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_inicio < 0) {
        perror("Erro ao criar a memoria para salvar Inicio.\n");
        exit(1);
    }
    inicioRT = (int*)shmat(mem_inicio, 0, 0);


    mem_duracao = shmget(duracao, sizeof(int), IPC_CREAT | IPC_EXCL | S_IWUSR | S_IRUSR);
    if(mem_duracao < 0) {
        perror("Erro ao criar a memoria para salvar Duracao.\n");
        exit(1);
    }
    duracaoRT = (int*)shmat(mem_duracao, 0, 0);

    /* Abre o arquivo */

    exec = fopen("exec.txt", "r");
    if(exec == NULL) {
        perror("Erro ao abrir o arquivo.");
    }

    /* Verifica algum erro na criacao/execucao do escalonador */

    if((escalonador_PID = fork()) == -1) {
        perror("Erro ao criar o escalonador.\n");
    }

    else if(escalonador_PID == 0) {
        execv("escalonador", argv);
    }

    else {
        printf("Escalonador foi criado.\n");
    }
    

    *f_arq = 0;

    /* Comeca a ler o arquivo. */

    while(fgets(linha_Lida,25,exec) != NULL) {
        
        espacos = 0;
        k = 0;
        linha_Lida[strlen(linha_Lida) - 1] = '\0';

        printf("Linha lida: %s \n", linha_Lida);
        

        for(i = 0; i < strlen(linha_Lida) - 1; i++) {
            if(linha_Lida[i] == ' ') {
                espacos++;
            }
        }

        // Caso RR
        if(espacos == 1) {            
            *tipo = 1;

            for(j = 4; j < 6; j++, k++) {
                nome[k] = linha_Lida[j];
            }
            nome[k] = '\0';

        }
        

        // Caso Prioridades
        else if(espacos == 2) {          
            *tipo = 2;

            for(j = 4; j < 6; j++, k++) {
                nome[k] = linha_Lida[j];
            }
            nome[k] = '\0';

            *prioridade = linha_Lida[10] - '0';
        }
        

        // Caso Real-Time
        else if(espacos == 3) {
            *tipo = 3;

            for(j = 4; j < 6; j++, k++) {
                nome[k] = linha_Lida[j];
            }
            nome[k] = '\0';
            k = 0;

            j = 9;
            if(linha_Lida[j] == 'P') {
                while(linha_Lida[j] != ' ') {
                    prog_dep[k] = linha_Lida[j];
                    j++;
                    k++;
                }
                prog_dep[k] = '\0';
                k = 0;
                *trig = 1;

                j = 14;
                if(linha_Lida[j+1] == '\0'){
                    *duracaoRT = (linha_Lida[j] - '0');
                }
                else {
                    *duracaoRT = 10*(linha_Lida[j] - '0') + (linha_Lida[j+1] - '0');
                }
                
            }

            else {

                j = 9;
                if(linha_Lida[j+2] == ' '){
                    *inicioRT = 10*(linha_Lida[j] - '0') + (linha_Lida[j+1] - '0');
                }
                else {
                    *inicioRT = (linha_Lida[j] - '0');
                }


                while(linha_Lida[j] != '=') {
                    j++;
                }
                j++;


                if(linha_Lida[j+1] == '\0'){
                    *duracaoRT = (linha_Lida[j] - '0');
                }
                else {
                    *duracaoRT = 10*(linha_Lida[j] - '0') + (linha_Lida[j+1] - '0');
                }
            }
        }
        
        *n_linha = 1;
        sleep(1);
        
    }
    *f_arq = 1;
    fclose(exec);

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



}