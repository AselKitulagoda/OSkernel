#include "libc.h"
#include "factory.h"
#define noOfPhilo 16


extern void printi();


void pThinking(int current){
    write(STDOUT_FILENO,"Philosopher ",12);
    printi(current);
    write(STDOUT_FILENO, " thinking", 9);
    PL011_putc(UART0,'\n',true);
}
    
void pEating(int current){
    write(STDOUT_FILENO,"Philosopher ", 12);
    printi(current);
    write(STDOUT_FILENO," eating",7);
    PL011_putc(UART0,'\n',true);
}

       void main_factory(){
           sem_t* forks=(sem_t*) sem_open();
           for (int i=0; i<noOfPhilo;i++){
               int phil = fork();
               if (phil == 0){
                while (1){
               sem_wait(&(forks[i]));
               sem_wait(&(forks[(i+1)%16]));
               pEating(i);
               sem_post(&(forks[i]));
               sem_post(&(forks[(i+1)%16]));
               yield();
               pThinking(i);
                }
               }
           }
           while(1);
       }

