#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

int main() {
    char* stProcess= "PROCESSO N I/O-bound "; //TODO alterar mensagem de acordo com cada processo   
    
    long long int i, pot, k=2;
    
    while(1) {
        pot = k;
        i=1;
        do {
            i++;
            pot *= k;
            usleep(9500);
            printf("%s pow(%lld,%lld) = %lld\n",stProcess,k,i,pot);
        }while(pot > 0);
        k++;
        if(k < 2){
            k=2;
        }
    }
    return 0;
}
