#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "PL011.h"
#include "libc.h"
#include "hilevel.h"

// int currentPhi=0;
// int x = 0;


// extern void put_i();
// extern void print();
    
// void pThinking(int current){
//     write(STDOUT_FILENO,"Philosopher ",12);
//     put_i(current);
//     write(STDOUT_FILENO, "thinking", 9);
//     PL011_putc(UART0,'\n',true);
// }
    
// void pEating(int current){
//     write(STDOUT_FILENO,"Philosopher ", 12);
//     put_i(current);
//     write(STDOUT_FILENO,"eating",7);
//     PL011_putc(UART0,'\n',true);
// }
    
// void main_philosopher(){
//     int x = id();
//     int  current = x-1;
//     chanWrite(current-1,2,current);
//     while (1){
//         switch(currentPhi){
//             case 0:
//                 currentPhi = chanRead(current-1,current);
//                 yield();
//                 break;
//             case 1:
//                 pThinking(current);
//                 chanWrite(current-1,2,current);
//                 currentPhi=0;
//                 yield();
//                 break;
//             case 2:
//                 pEating(current);
//                 chanWrite(current-1,3,current);
//                 currentPhi = 0;
//                 yield();
//                 break;
//             default:
//                 currentPhi = 0;
//                 chanWrite(current-1,2,current);
//                 yield();
//                 break;
                
//         }
        
//     }
//     exit(EXIT_SUCCESS);
    
//}