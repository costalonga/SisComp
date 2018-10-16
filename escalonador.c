#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include "prog_aux.h"
#include "fila.h"
#define BUF_SIZE 17
#define TAM_NOME 3
#define MAX_PROGS 60

/*
   set follow-fork-mode child
   set detach-on-fork off
*/

//
int fd[2];

// Usada para terminar o programa quando o .txt for completamente lido.
void handler(int sinal);

// Usada para imprimir o nome dos programas no vetor de RT (TESTE).
void imprime(struct carac_progs v[], int cont);

// Imprime o vetor de segundos (TESTE).
void imprime_seg(bool v[]);


int main() {

    /* ---------------------- VARIAVEIS ESCALONADOR ---------------------- */

    // Variáveis que serão usadas para calcular o minuto.
    time_t start = time(NULL);
    double tempoAtual;

    // Variáveis relacionadas ao pipe.
    ssize_t d_TR, d_RE;

    // Variável que vai armazenar o tipo de processo.
    int tipo;

    // String em que armazena as linhas do exec.txt
    char buffer[BUF_SIZE];
    

    // Struct para armazenar os dados dos programas.
    carac_progs st;
    pFila p;

    // Variáveis Real-Time.
    int ini = 0;
    int dur = 0;
    int cont = 0; // Contador da qtd. de structs no vetor.

    // Vetor que armazena o programas rodando RT.
    struct carac_progs prog_RodandoRT[MAX_PROGS];

    // Vetor que armazena os segundos ja ocupados.
    bool vec_Segundos[60];

    // Variavel utilizada para verificar se o processo RT pode rodar.
    bool permissao = false;

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
        //__ssize_t qtd_Char;
        __ssize_t read;
        size_t len = 0;
        int fim;

        /* ------- VARIAVEIS INTERPRETADOR ------- */

        if((arq = fopen("fila_Programas.txt","r")) == NULL){
            printf("Interpretador nao abriu o arquivo. Saindo do programa.\n");
            exit(1);
        }

        // Transmitindo dados para o escalonador (linha por linha).

        //
        while(fgets(buffer, BUF_SIZE, arq) != NULL) {

            if(strlen(buffer) == 1) {

            }

            else {
                d_TR = write(fd[1], buffer, strlen(buffer) + 1);
                
                sleep(1);
            }
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

    // Inicializa o vetor de segundos, e vetor contendo os RT.
    ini_Vetor(vec_Segundos, prog_RodandoRT);

    // Fila dos processos PRONTOS para executar.
    struct Fila* f = cria_Fila();



    // while deve terminar quando nou houver mais processos executanto - arrumar depois.
    while(1) {

        // Atualiza o contador do tempo.
        tempoAtual = (double)time(NULL) - start;
        
        // Reinicia o minuto.
        if(tempoAtual > 60) {
            start = time(NULL);
        }

        // Le a pipe.
        d_RE = read(fd[0], buffer , sizeof(buffer));

        // Retorna o tipo do processo.
        tipo = analisa_buffer(buffer);

        // Estrutura que armazena informaçoes para o fila de prontos
        struct No* n;
        
            switch(tipo) {
                case 1:
                    // Obtem inicio e duração do RT.
                    st = analisa_RealTime(prog_RodandoRT, buffer, tipo, cont);

                    // Salva no vetor de RT  
                    salva_no_Vetor(st, prog_RodandoRT, cont);   
                    cont++; 

                    // Recebe infos p colocar na fila de prontos (nome e PR) 
                    p = pFila_RT(st);     
                    
                    // Coloca na fila de prontos
                    insere_FilaProntos(f, p.nome, p.PR);

                    // Ordena a fila de acordo com as prioridades.           
                    ordena_Prioridades(f);          

                    // Remove o primeiro elemento (maior prioridade)                
                    n = remove_FilaProntos(f);                      

                    // Verifica a fila de espera para ver se tem algum programa para ser executado.
                    // Se tiver, executa e segue o programa

                    // Se nao tiver, segue o programa

                    // Se for RT
                    if(n->PR == -1) {

                        // Verifica tempo de inicio e duracao no vetor dos RT.
                        verifica_vecRT(n, st, cont, prog_RodandoRT);  

                        // Verifica se o programa pode rodar no tempo desejado.  
                        permissao = verificaRT_vetorSegundos(vec_Segundos, st.inicio, st.duracao+st.inicio);
                        
                        // Se puder..
                        if(permissao == true) {

                            // Verifica se pode rodar no min. atual. 
                            permissao = verifica_minAtual(tempoAtual, st.inicio);

                            // Se sim, executa. 
                            if(permissao == true) {
                                printf("Programa que sera executado: %s   De: %ld s ate %ld s.\n", st.nome, st.inicio, st.duracao + st.inicio);

                                // Executa
                                // Adiciona na lista de executando
                            }

                            // Senao, entra na lista de espera
                            else {
                                printf("Lista de espera, vou executar no proximo minuto.\n");

                                // Adiciona na lista de espera
                            }
                        
                        }

                        else {
                            // Adiciona a lista de bloqueados. (não podem executar). 
                        }

                    }

                    // Se for RR
                     else if(n->PR == 8) {
                         // Procura um "buraco" para executar o RR

                         // Se houver, executa e coloca na lista de terminados

                     }

                    // Se for PR
                     else {
                         // Caclcula o tempo que o processo sera executado

                     }
                    break;

                case 2:
                    p = pFila_PR(buffer);
                    insere_FilaProntos(f, p.nome, p.PR);
                    ordena_Prioridades(f);
                    break;

                case 3:
                    p = pFila_RR(buffer);
                    insere_FilaProntos(f, p.nome, p.PR);
                    break;
            }
        
              //imprime_seg(vec_Segundos);
    }
}



void handler(int sinal) {
    printf("O arquivo terminou. Fechando o canal de comunicacao.\n");
    close(fd[1]);
    sleep(10);
}

void imprime(struct carac_progs v[], int cont){

    int i;
    for(i = 0; i < cont; i++) {
        printf("Valor da posicao %d: %s\n", i, v[i].nome);
    }
}

void imprime_seg(bool v[]) {
    
    int i;

    for(i = 0; i < 60; i++) {
        printf("Valor do segundo %d: %d\n", i, v[i]);
    }
}