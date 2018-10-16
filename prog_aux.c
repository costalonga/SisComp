#include "prog_aux.h"
#include "fila.h"

// Chamada na main
int analisa_buffer(char s[]) {

    int tipo;
    int i = 7;
    char aux[18];
    strcpy(aux, s);

    if(aux[i] == 'I' || aux[i] == 'i') {
        tipo = 1;       // Real-Time
        return tipo;
    }

    else if(aux[i] == 'P' || aux[i] == 'p') {
        tipo = 2;
        return tipo;    // Prioridades
    }

    else
        return 3;


    
}

/* ------------------- ------------------- */
// Chamada na main
carac_progs analisa_RealTime(struct carac_progs v[], char buf[], int tipo, int cont) {

    carac_progs st;
    int i, a = 0;
    char dur_Char[2];
    char ini_Char[2];
    char ini_Prog[2];
    char nome[TAM_NOME];
    char* endptr;

    st.tipo = tipo;
   
    i = 4;

    while(buf[i] != ' ') {      
        nome[a] = buf[i];
        a++;
        i++;
    }

    strcpy(st.nome, nome);
    st.nome[a] = '\0';

    a = 0;
    i = i + 3;

    while(buf[i] != ' ') {
        ini_Char[a] = buf[i];
        a++;
        i++;
    }

    if(ini_Char[0] == 'p' | ini_Char[0] == 'P') {
        acessa_vec_Struct(v, ini_Char, &st.inicio, cont);
    }

    else {
        st.inicio = strtoimax(ini_Char, &endptr, 10);
    }
    
    a = 0;
    i = i +3;

    while(buf[i] != '\0') {
        if(a == 2) {
            break;
        }

        dur_Char[a] = buf[i];
        a++;
        i++;
            
    }

    st.duracao = strtoimax(dur_Char, &endptr, 10);

    return st;
}

/* ------------------- ------------------- */
// Chamada na main.
void ini_Vetor(bool vec[60], struct carac_progs v[]) {

    int x;

    for(x = 0; x < 60; x++) 
        vec[x] = false;

    for(x = 0; x < 60; x++) {
        strcpy(v[x].nome, "NULL");
        v[x].inicio = 0;
        v[x].duracao = 0;
        v[x].tipo = -1;
    }
}

/* ------------------- ------------------- */
// Chamada na analisa_RealTime
void acessa_vec_Struct(struct carac_progs v[], char s[], intmax_t* ini, int cont) {

    int i;

    if(cont == 0) {
        printf("O vetor estava vazio.\n");
    }
    
    for(i = 0; i < cont; i++) {
        if(strcmp(v[i].nome,s) == 0) {
            *ini = v[i].inicio + v[i].duracao + 1; 
            return;
        }
    }
    
}

/* ------------------- ------------------- */
// Chamada na main
void salva_no_Vetor(carac_progs st, struct carac_progs v[], int cont) {

        if(cont == 60) {
            printf("Vetor cheio. \n");
            return;
        }
        
        strcpy(v[cont].nome, st.nome);
        v[cont].inicio = st.inicio;
        v[cont].duracao = st.duracao;
        v[cont].tipo = st.tipo;
        
    }

/* ------------------- ------------------- */

pFila pFila_PR(char buf[]) {

    char nome[TAM_NOME];
    int i, a = 0;
    pFila p;
    char PR[2];
    char* endptr;

    i = 4;

    while(buf[i] != ' ') {
        nome[a] = buf[i];
        i++;
        a++;
    }
    nome[a] = '\0';
    
    strcpy(p.nome, nome);

    i = i + 4;
    a = 0;

    PR[a] = buf[i];
    PR[++a] = '\0';

    p.PR = strtoimax(PR, &endptr, 10);

    return p;



}

/* ------------------- ------------------- */

pFila pFila_RT(carac_progs st) {

    pFila p;

    strcpy(p.nome, st.nome);        // Cuidado com \0
    p.PR = -1;

    return p;
}

/* ------------------- ------------------- */

pFila pFila_RR(char buf[]) {

    char nome[TAM_NOME];
    int i, a = 0;
    pFila p;
    char* endptr;

    i = 4;
    
    while(i < (strlen(buf) - 1)) {
        nome[a] = buf[i];
        i++;
        a++;
    }

    nome[a] = '\0';
    strcpy(p.nome, nome);

    p.PR = 8;

    return p;
}

/* ------------------- ------------------- */

bool verificaRT_vetorSegundos(bool v[], intmax_t ini, intmax_t fim) {

    intmax_t i;
    if(ini != 0) {
        ini--;
    }


    if(v[ini] == true) {
        printf("Ja existe algum processo rodando no tempo solicitado, descartando processo.\n");
        return false;
    }

    else if(fim > 60) {
        printf("Limite de tempo excedido pelo programa.\n");
        return false;
    }

    else {
        for(i = ini; i < fim; i++) {
            if(v[i] == true) {
                printf("Ja existe algum processo rodando no tempo solicitado, descartando processo.\n");
                return false;
            }
        }
    }

    for(i = ini; i < fim; i++) {
        v[i] = true;
    }
        return true;
    
}

/* ------------------- ------------------- */

void verifica_vecRT(struct No* n, carac_progs c, int cont, carac_progs v[]) {

    int i;
   
    strcpy(c.nome,n->nome);
    for(i = 0; i < cont; i++) {
        if(strcmp(v[i].nome, n->nome) == 0) {
            c.inicio = v[i].inicio;
            c.duracao = v[i].duracao;
            return;
        }
    }

    printf("Struct correspondente nao encontrada no vetor de RT.\n");
    return;
}



bool verifica_minAtual(double t, intmax_t ini) {

    if(t > ini) {
        return false;
    }

    else {
        return true;
    }
}
