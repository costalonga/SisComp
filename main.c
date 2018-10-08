#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "prog_aux.h"
#define BUF_SIZE 25
#define TAM_NOME 3

/*


 TODO: Estrutura que armazena os arquivos
 TODO: Arrumar funçao que retorna a struct do prog_aux.c


*/



/*
   set follow-fork-mode child
   set detach-on-fork off
*/



// Usada para terminar o programa quando o .txt for completamente lido.
void handler(int sinal);



int main() {

    /* ---------------------- VARIAVEIS ESCALONADOR ---------------------- */

    // Variáveis relacionadas ao pipe.
    ssize_t d_TR, d_RE;
    int fd[2];

    // Variável que vai armazenar o tipo de processo.
    int tipo;

    // String em que armazena as linhas do exec.txt
    char buffer[BUF_SIZE];

    // Struct para armazenar os dados dos programas.
    carac_Progs st;

    // Variáveis Real-Time.
    int ini = 0;
    int dur = 0;

    /* ---------------------- VARIAVEIS ESCALONADOR ---------------------- */


    if(buffer == NULL) {
        printf("Erro alocando memoria. Saindo do programa.\n");
        exit(-1);
    }



    // Criando canal de comunicação entre escalonador e interpretador.
    if(pipe(fd) < 0) {
        printf("Erro ao criar a pipe. Saindo do programa.\n");
        exit(-1);
    }


    // Criando o interpretador.
    if(fork() == 0) {
        close(fd[0]);

        /* ------- VARIAVEIS INTERPRETADOR ------- */

        FILE *arq;
        __ssize_t qtd_Char;
        int fim;

        /* ------- VARIAVEIS INTERPRETADOR ------- */

        if((arq = fopen("fila_Programas.txt","r")) == NULL){
            printf("Interpretador nao abriu o arquivo. Saindo do programa.\n");
            exit(1);
        }

        // Transmitindo dados para o escalonador (linha por linha).


        while(fgets(buffer, sizeof(buffer), arq) != NULL) {

            d_TR = write(fd[1], buffer, strlen(buffer) + 1);
            sleep(1);
        }

        // Avisa o escalonador que o arquivo terminou.
        fim = kill(0, SIGUSR1);
        fclose(arq);
        if(fim == -1) {
            printf("Erro ao enviar o sinal.\n");
        }

    }
    // Fim do interpretador.



    // Recebe o sinal do interpretador para terminar.
    signal(SIGUSR1, handler);

    close(fd[1]);

    while(d_RE != -1) {
        d_RE = read(fd[0], buffer , sizeof(buffer));
        printf("Programa lido (escalonador): %s\n", buffer);

        ini_Vetor(vec_Segundos);
        tipo = analisa_buffer(buffer);
        //preenche_Struct(st);
        //insere_Fila(st);

        switch(tipo) {
            case 1:
                printf("Real-Time.\n");
                st = analisa_RealTime(buffer, tipo);
                printf("Nome RT: %s   INI: %d   DUR: %d   \n", st.nome, st.inicio, st.duracao);
                break;

            case 2:
                printf("Prioridades.\n");
                break;

            case 3:
                printf("RR.\n");
		        break;
        }
    }
}



void handler(int sinal) {
    printf("O arquivo terminou. Fechando o programa.\n");
    exit(1);
}

