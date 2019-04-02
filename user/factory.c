#include "libc.h"
#include "factory.h"
// #include "hilevel.h"
#define noOfPhilo 16


// ph1_t ph1[noOfPhilo];
sem_t* forks[noOfPhilo+1];





// extern void main_philosopher();
extern void put_i();
// extern void print();

// int state = 0;
// int eat = 0 ;

// void philosopher_eaten(int current){
//     write(STDOUT_FILENO,"Philosopher ", 12);
//     put_i(current);
//     write(STDOUT_FILENO," has eaten", 10);
//     print('\n');
    
// }

// void main_factory(){
//     for (int i=0;i<noOfPhilo;i++){
//         int child = fork();
//         if (child==0){
//             exec(&main_philosopher);
//         }
//         else{
//             pipe(i,0,0,child-1);
//         }
//     }
    
//     while(1){
//         for (int i=0;i<noOfPhilo;i++){
//             switch (state){
//                 case 0:
//                     state = chanRead(i,0);
//                     yield();
//                     break;
//                 case 1:
//                     chanWrite(i,1,0);  //think
//                     state = 0;
//                     yield();
//                     break;
//                 case 2:
//                     chanWrite(i,2,0);  //eat
//                     state=0;
//                     yield();
//                     break;
//                 default:
//                     chanWrite(i,1,0);  //think
//                     state=0;
//                     yield();
//                     break;
//             }
//         }
//     }
    

/// }
//

void pThinking(int current){
    write(STDOUT_FILENO,"Philosopher ",12);
    put_i(current);
    write(STDOUT_FILENO, " thinking", 9);
    PL011_putc(UART0,'\n',true);
}
    
void pEating(int current){
    write(STDOUT_FILENO,"Philosopher ", 12);
    put_i(current);
    write(STDOUT_FILENO," eating",7);
    PL011_putc(UART0,'\n',true);
}

void initialiseForks(){
    for(int i=0;i<16;i++){
        forks[i] = (sem_t*) 1;
    }
}



       void main_factory(){
           initialiseForks();
           for (int i=0; i<noOfPhilo;i++){
               int phil = fork();
               if (phil == 0){
                while (1){
               sem_wait(forks[i]);
               sem_wait(forks[(i+1)%16]);
               pEating(i);
               sem_post(forks[i]);
               sem_post(forks[(i+1)%16]);
               yield();
               pThinking(i);
                }
               }
               
               
               
               
               
               
               
               
               
               
           }
       }

