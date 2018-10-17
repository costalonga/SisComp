#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processos.h"
#include "listas.h"

struct node {
   Processo *data; 
   int key;
   //int data;
   Node *next;
} ;

struct lista {
    Node* head;
    Node* current;
    
    int tempo;  //TODO func pra retornar tempo total 
    int Nelem;
    
};


Lista* lista_cria(){
    Lista *l = (Lista *)malloc(sizeof(Lista));
    
    if(l == NULL){
        printf("Erro malloc lista\n");
        exit(1);
    }
    
    l->Nelem = 0;
    l->head = NULL;
    l->current = NULL;
    return l;
}


//insert link at the first location
void insere_lista(Lista *lst, Processo* data) { //data
  
    Node *ptr;
    Node *novo = (Node*) malloc(sizeof(Node));
    
    if(novo == NULL){
        printf("Erro malloc node\n");
        exit(1);
    }

    lst->Nelem++;
    novo->data = data;
    novo->key = (lst->Nelem - 1);
    novo->next = NULL;
    
    if (lst->head == NULL) { //primeiro elemento a ser inserido
        lst->head = novo;
    }

    else {
        ptr = lst->head;
        while(ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = novo;
    } 
}


//display the list
void printList(Lista *lst) {
    Node *ptr = lst->head;
    printf("\n[ ");

    //start from the beginning
    while(ptr != NULL) {
      printf("(Key: %d, Inicio: %d)\n ",ptr->key,ptr->data->ini); //data
      ptr = ptr->next;
    }
    printf(" ]\n");
}

//delete first item
Node* lista_extractFirst(Lista *lst) {
    
    //save reference to first link
    Node *tempLink = lst->head;
    
    //mark next to first link as first 
    lst->head = lst->head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
int isEmpty(Lista *lst) {
    if (lst->head == NULL){
        return 1;
    }
    return 0; 
}

int length(Lista *lst) {
   return lst->Nelem;
}


//delete a link with given key
void lista_delete(Lista *lst, int key) {
    
    //if list is empty
    if(lst->head == NULL) {
        exit(1);
    }
    
    if(lst->Nelem <= key) {
        printf("Lista possui apenas %d elementos, impossivel remover!",lst->Nelem);
        exit(1);
    }
    
    //start from the first link
    Node* ptr = lst->head;
    Node* previous = NULL;
    
    //navigate through list
    while(ptr->key != key) {
        
        //if it is last node
        if(ptr->next == NULL) {
            exit(1);
        }
         
        else {
            previous = ptr; //store reference to current link
            ptr = ptr->next; //move to next link
        }
    }

    //found a match, update the link
    if(ptr == lst->head) {
        lst->head = lst->head->next; //change first to point to next link
    } 
    else {
        previous->next = ptr->next; //bypass the current link
    }    
    
    lst->head = ptr;
    lst->Nelem--;
}

/*
int tempoTotal (Lista *lst) {

    int sum = 0;
    Node *ptr = lst->head;
    
    if(isEmpty(lst)) {
        return 0;
    }
    
    while(ptr->next != NULL){
        sum += ptr->data->duracao;
        ptr = ptr->next;
    } 
    return sum;
}
*/
/*
//find a link with given key
Node* find(int key) {
   //start from the first link
   Node* current = head;
   //if list is empty
   if(head == NULL) {
      return NULL;
   }
   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}
*/