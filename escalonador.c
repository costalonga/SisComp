#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "prog_aux.h"
#include "fila.h"
#define BUF_SIZE 17
#define TAM_NOME 3
#define MAX_PROGS 60

/*


 TODO: Estrutura que armazena os arquivos
 TODO: Condição de termino do programa (nao qnd txt for lido)


*/



/*
   set follow-fork-mode child
   set detach-on-fork off
*/



// Usada para terminar o programa quando o .txt for completamente lido.
void handler(int sinal);

// Usada para imprimir o nome dos programas no vetor de RT (TESTE).
void imprime(struct carac_progs v[], int cont);


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
    carac_progs st;

    // Variáveis Real-Time.
    int ini = 0;
    int dur = 0;
    int cont = 0; // Contador da qtd. de structs no vetor.

    // Vetor que armazena o programas rodando.
    struct carac_progs prog_Rodando[MAX_PROGS];

    // Vetor que armazena os segundos ja ocupados.
    bool vec_Segundos[60];

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

    ini_Vetor(vec_Segundos, prog_Rodando);

    while(d_RE != -1) {
        d_RE = read(fd[0], buffer , sizeof(buffer));

        tipo = analisa_buffer(buffer);

        switch(tipo) {
            case 1:
                printf("Real-Time.\n");
                st = analisa_RealTime(prog_Rodando, buffer, tipo, cont);
                salva_no_Vetor(st, prog_Rodando, cont);     // Vetor usado para checar o inicio e duracao do programa.
                // Coloca na fila.
                // Ordena fila baseado nas prioridades.
                // Verifica primeiro da fila (pode ou não rodar?)
                // Se pode executar, executa.
                printf("Nome RT: %s   INI: %d   DUR: %d   \n", st.nome, st.inicio, st.duracao);
                cont++;
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

void imprime(struct carac_progs v[], int cont){

    int i;
    for(i = 0; i < cont; i++) {
        printf("Valor da posicao %d: %s\n", i, v[i].nome);
    }
}
