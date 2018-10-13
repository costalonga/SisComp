#include "prog_aux.h"

// Chamada na main
int analisa_buffer(char s[]) {

    int tipo;
    int i = 5;
    char aux[25];
    strcpy(aux, s);

    while(aux[i] != ' ') {
        i++;
    }

    i++;

    if(aux[i] == 'I' || aux[i] == 'i') {
        tipo = 1;       // Real-Time
        return tipo;
    }

    else if(aux[i] == 'P' || aux[i] == 'p') {
        tipo = 2;
        return tipo;    // Prioridades
    }

    //Else Round Robin
    tipo = 3;       // RR
    return tipo;
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
        // Caso mude o tamanho do vetor, ajeitar aqui pra não dar erro na struct.
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

    for(i = 0; i < cont; i++) {
        if(strcmp(v[i].nome,s) == 0) {
            *ini = v[i].inicio + v[i].duracao; 
            return;
        }
    }

    printf("O vetor estava vazio.\n");
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



