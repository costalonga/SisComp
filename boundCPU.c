#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

#define SIZE 900000
#define OUTER_LOOP 1000

int main() {
    char* stProcess= "PROCESSO 1 CPU"; //TODO alterar mensagem de acordo com cada processo   
    
    long long int fibo[SIZE];
    long long int c1,c2,c3;
    long long int i,j,k;
    
    j=0;
    while(1){
        k=0;
        while(k < OUTER_LOOP){
            c1=c3=0; 
            c2=1;
            fibo[0]=c1;
            fibo[1]=c2;
            for(i=2; i < SIZE; i++) {
                c3 += abs(c1 + c2);
                c1 = c2;
                c2 = c3;
                fibo[i] = c3;
                //usleep(100);
                //printf("i: %lld, f: %lld\n",i,fibo[i]);
            }
            //printf("DONE %s i: %lld, f: %lld\n",stProcess,i,fibo[i]);
            k++;
        }
        if((i-j) >= 0){
            j++;
        }
        else{
            j=0;
        }
        printf("%s fibonacci number#%lld =  %lld\n",stProcess,(i-j),fibo[i-j]);
    }
    return 0;
}
