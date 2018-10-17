#ifndef PROG_PRINCIPAIS_LISTAS_H
#define PROG_PRINCIPAIS_LISTAS_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processos.h"

typedef struct node Node;
typedef struct lista Lista;

Lista* lista_cria();

//display the list
void printList(Lista *lst);

//insert link at the first location
void insere_lista(Lista *lst, Processo* data);

//delete first item
Node* lista_extractFirst(Lista *lst);

//is list empty
int isEmpty(Lista *lst);

//find a link with given key
Node* find(Node *ptr, int key);

//delete a link with given key
void lista_delete(Lista *lst, int key);

//get total time of all process in the list, can be used as waiting list or for processes that are being executed
int tempoTotal(Lista *lst);

#endif //PROG_LISTA_H