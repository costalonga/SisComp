#include "prog_aux.h"


int analisa_buffer(char* s) {

    int tipo;
    int i = 5;
    char* aux = (char*)malloc(sizeof(char)*30);
    aux = s;

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

void preenche_Struct(carac_Progs cp) {

}

/* ------------------- ------------------- */

void insere_Fila(carac_Progs cp) {

}

/* ------------------- ------------------- */

carac_Progs analisa_RealTime(char buf[25], int tipo) {

    carac_Progs st;
    int i, a = 0;
    char dur_Char[2];
    char ini_Char[2];

    st.tipo = tipo;

    for(i = 4; buf[i] != ' '; i++) {
        st.nome[a] = buf[i];
        a++;
    }
    printf("nome: %s\n", st.nome);

    a = 0;
    i = i + 3;

    for(i = 9; buf[i] != ' '; i++) {
        ini_Char[a] = buf[i];
        a++;
    }

    printf("ini char: %s\n", ini_Char);
    st.inicio = atoi(ini_Char);

    a = 0;
    i = i +3;

    for(i = i; buf[i] != ' '; i++) {
        dur_Char[a] = buf[i];           // SEGFAULT no ultimo teste??
        a++;
    }

    st.duracao = atoi(dur_Char);

    return st;


}

void ini_Vetor(bool vec[60]) {

    int x;

    for(x = 0; x < 60; x++) {
        vec[x] = false;
    }
}



