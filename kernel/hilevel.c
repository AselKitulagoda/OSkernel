
/* Copyright (C) 2017 Daniel Page <csdsp@bristol.ac.uk>
 *
 * Use of this source code is restricted per the CC BY-NC-ND license, a copy of
 * which can be found via http://creativecommons.org (and should be included as
 * LICENSE.txt within the associated archive or repository).
 */

#include "hilevel.h"


//Coursework

typedef int sem_t;

typedef struct {
    int pid;
    sem_t* mutex;
} waitlist;

waitlist wlist[1000];


pcb_t pcb[ 1000 ]; pcb_t* current = NULL;pcb_t* prev = NULL;pcb_t* next = NULL;
int processesRunning=0; uint32_t toTos[1000]; int currentlyExecuting=0;int count = 0;
channel channels[16];

void put_i(int r){

    if (r < 10){
        PL011_putc(UART0, r + '0', true);
    }
    else{
        PL011_putc(UART0, '1', true);
        PL011_putc(UART0, (r-10) + '0', true);
    }
}







void dispatch( ctx_t* ctx, pcb_t* prev, pcb_t* next ) {
  char prev_pid = '?', next_pid = '?';

  if( NULL != prev ) {
    memcpy( &prev->ctx, ctx, sizeof( ctx_t ) ); // preserve execution context of P_{prev}
    prev_pid = '0' + prev->pid;
  }
  if( NULL != next ) {
    memcpy( ctx, &next->ctx, sizeof( ctx_t ) ); // restore  execution context of P_{next}
    next_pid = '0' + next->pid;
  }

    PL011_putc( UART0, '[',      true );
    PL011_putc( UART0, prev_pid, true );
    PL011_putc( UART0, '-',      true );
    PL011_putc( UART0, '>',      true );
    PL011_putc( UART0, next_pid, true );
    PL011_putc( UART0, ']',      true );

    current = next;                             // update   executing index   to P_{next}

  return;
}

int findmax(){
    int length  = sizeof(pcb)/sizeof(pcb[0]);
    int currenthighest = -1;
    int highestindex = -5;
    for (int i=0;i<length;i++){
        if ((pcb[i].priority>currenthighest) && pcb[i].status!=STATUS_TERMINATED){
        currenthighest = pcb[i].priority;
        highestindex = i;
}
}
    return highestindex;
}


void schedule( ctx_t* ctx ) {
    int length  = sizeof(pcb)/sizeof(pcb[0]);
    char test = '0' + length;
//     for (int i=0;i<length;i++){
//         PL011_putc( UART0, test, true );
//         if (current->pid == pcb[i].pid){
//             dispatch(ctx,&pcb[i],&pcb[(i+1)%length]);
//             pcb[i].status = STATUS_READY;
//             pcb[(i+1)%length].status = STATUS_EXECUTING;
//             break;
//         }
//
//     }
//
       int arghighestpri = findmax();
       while (processesRunning>0 && current->pid==0 && arghighestpri==0){
             for (int i =0;i<length;i++){
       if (pcb[i].pid != current->pid){
           pcb[i].priority+=pcb[i].prioritychange;
       }
       }
           arghighestpri=findmax();
       }
       dispatch(ctx,current,&pcb[(arghighestpri)]);
       current->status = STATUS_READY;
       currentlyExecuting = arghighestpri;
       
       pcb[arghighestpri].status=STATUS_EXECUTING;
       current->priority = current->initialpriority;
   for (int i =0;i<length;i++){
       if (pcb[i].pid != current->pid){
           pcb[i].priority+=pcb[i].prioritychange;
       }
   }


  return;
}





extern void     main_P3();
extern uint32_t tos_P3;
extern void     main_P4();
extern uint32_t tos_P4;
extern void     main_P5();
extern uint32_t tos_P5;
extern void main_console();
extern uint32_t tos_console;
extern void main_factory;
extern void main_philosopher;

uint32_t findtos(int i){
  uint32_t tos = 0;
  if (i==2){
    tos = (uint32_t)&tos_P4;
  }
  else if (i==3){
    tos = (uint32_t)&tos_P5;
  }
  else if (i==1){
    tos = (uint32_t)&tos_P3;
  }
  toTos[i]=tos;
  return tos;
}

void setpri(int i){
int priorities[] = {1,5,10};
int prioritychange[]={4,5,2};
pcb[i].priority = priorities[i];
pcb[i].prioritychange=prioritychange[i];
}

void hilevel_handler_rst(ctx_t* ctx) {
  /* Configure the mechanism for interrupt handling by
   *
   * - configuring timer st. it raises a (periodic) interrupt for each
   *   timer tick,
   * - configuring GIC st. the selected interrupts are forwarded to the
   *   processor via the IRQ interrupt signal, then
   * - enabling IRQ interrupts.
   */

//   for (int i=0;i<sizeof(pcb_t);i++){
//       memset(&pcb[i],i,sizeof(pcb_t));
//       pcb[i].pid = i+3;
//       pcb[i].status = STATUS_CREATED;
//       pcb[i].ctx.cpsr=0x50;
//       if (i == 0){
//       pcb[i].ctx.pc= (uint32_t)(&main_P3);
//       pcb[i].ctx.sp=(uint32_t)(&tos_P3);

//       }
//       if (i == 1){
//       pcb[i].ctx.pc= (uint32_t)(&main_P4);
//       pcb[i].ctx.sp=(uint32_t)(&tos_P4);
//     }
//        if (i == 2){
//       pcb[i].ctx.pc= (uint32_t)(&main_P5);
//       pcb[i].ctx.sp=(uint32_t)(&tos_P5);
//       dispatch(ctx,NULL,&pcb[0]);
//       }
//   }
//
//
  PL011_putc(UART0,'R',true);
  memset( &pcb[ 0 ], 0, sizeof( pcb_t ) );     // initialise 0-th PCB = P_1
  pcb[ 0 ].pid      = 0;
  pcb[ 0 ].status   = STATUS_READY;
  pcb[ 0 ].ctx.cpsr = 0x50;
  pcb[ 0 ].ctx.pc   = ( uint32_t )( &main_console );
  pcb[ 0 ].priority = 25;
  pcb[ 0 ].initialpriority = 40;
  pcb[0].prioritychange = 5;
  pcb[ 0 ].ctx.sp   = ( uint32_t )( &tos_console  );

//   toTos[0] = (uint32_t)&tos_console;

//   memset( &pcb[ 1 ], 0, sizeof( pcb_t ) );     // initialise 1-st PCB = P_2
//   pcb[ 1 ].pid      = 4;
//   pcb[ 1 ].status   = STATUS_CREATED;
//   pcb[ 1 ].ctx.cpsr = 0x50;
//   pcb[ 1 ].ctx.pc   = ( uint32_t )( &main_P4 );
//   pcb[1].priority = 20;
//   pcb[1].initialpriority = 20;
//   pcb[1].prioritychange = 10;
//   pcb[ 1 ].ctx.sp   = ( uint32_t )( &tos_P4  );

//    memset( &pcb[ 2 ], 0, sizeof( pcb_t ) );     // initialise 1-st PCB = P_2
//   pcb[ 2 ].pid      = 5;
//   pcb[ 2 ].status   = STATUS_CREATED;
//   pcb[ 2 ].ctx.cpsr = 0x50;
//   pcb[ 2 ].ctx.pc   = ( uint32_t )( &main_P5 );
//   pcb[2].priority = 30;
//   pcb[2].initialpriority = 30;
//   pcb[2].prioritychange = 1;
//   pcb[ 2 ].ctx.sp   = ( uint32_t )( &tos_P5  );

//   pcb[ 2 ].pid      = 5;
//   pcb[ 2 ].status   = STATUS_CREATED;
//   pcb[ 2 ].ctx.cpsr = 0x50;
//   pcb[ 2 ].ctx.pc   = ( uint32_t )( &main_console );
//   pcb[2].priority = 30;
//   pcb[2].initialpriority = 30;
//   pcb[0].prioritychange = 1;
//   pcb[ 2 ].ctx.sp   = ( uint32_t )( &tos_console  );




  TIMER0->Timer1Load  = 0x00100000; // select period = 2^20 ticks ~= 1 sec
  TIMER0->Timer1Ctrl  = 0x00000002; // select 32-bit   timer
  TIMER0->Timer1Ctrl |= 0x00000040; // select periodic timer
  TIMER0->Timer1Ctrl |= 0x00000020; // enable          timer interrupt
  TIMER0->Timer1Ctrl |= 0x00000080; // enable          timer

  GICC0->PMR          = 0x000000F0; // unmask all            interrupts
  GICD0->ISENABLER1  |= 0x00000010; // enable timer          interrupt
  GICC0->CTLR         = 0x00000001; // enable GIC interface
  GICD0->CTLR         = 0x00000001; // enable GIC distributor

  /* Once the PCBs are initialised, we arbitrarily select the one in the 0-th
   * PCB to be executed: there is no need to preserve the execution context,
   * since it is is invalid on reset (i.e., no process will previously have
   * been executing).
   */
  dispatch( ctx, NULL, &pcb[0] );
  pcb[0].status = STATUS_EXECUTING;
  int_enable_irq();

  return;
}

void FindAvailable(){
    for (int i=0;i<count;i++){
        if (*(wlist[i].mutex) ==  1 ){
            pcb[wlist[i].pid].status = STATUS_READY;
            wlist[i].pid = -1;
            wlist[i].mutex = NULL;
        }
    }
}

void hilevel_handler_irq(ctx_t* ctx) {
  // Step 2: read  the interrupt identifier so we know the source.

  uint32_t id = GICC0->IAR;

  // Step 4: handle the interrupt, then clear (or reset) the source.

  if( id == GIC_SOURCE_TIMER0 ) {
      //check wait list here
//       PL011_putc( UART0, 'T', true );
    FindAvailable();
    schedule(ctx);
     TIMER0->Timer1IntClr = 0x01;
  }

  // Step 5: write the interrupt identifier to signal we're done.

  GICC0->EOIR = id;

  return;
}


void print(char* str){
    for (int i=0;i<strlen(str);i++){
        PL011_putc(UART0,str[i],true);
    }
}

//check through queue to see if semaphore value changed and if its 1 then remove from list 
//



void hilevel_handler_svc( ctx_t* ctx, uint32_t id ) {
  /* Based on the identifier (i.e., the immediate operand) extracted from the
   * svc instruction,
   *
   * - read  the arguments from preserved usr mode registers,
   * - perform whatever is appropriate for this system call, then
   * - write any return value back to preserved usr mode registers.
   */

  switch( id ) {
    case 0x00 : { // 0x00 => yield()
      schedule( ctx );

      break;
    }

    case 0x01 : { // 0x01 => write( fd, x, n )
      int   fd = ( int   )( ctx->gpr[ 0 ] );
      char*  x = ( char* )( ctx->gpr[ 1 ] );
      int    n = ( int   )( ctx->gpr[ 2 ] );

      for( int i = 0; i < n; i++ ) {
        PL011_putc( UART0, *x++, true );
      }

      ctx->gpr[ 0 ] = n;

      break;
    }
    case 0x03:{
      print("FORK");
      processesRunning +=1;
      memset(&pcb[processesRunning],0,sizeof(pcb_t));
      //creates child console
      memcpy(&pcb[processesRunning].ctx,ctx,sizeof(ctx_t));
      pcb[processesRunning].pid = processesRunning;
      pcb[processesRunning].status = STATUS_CREATED;
      pcb[processesRunning].ctx.cpsr = ctx->cpsr;
      pcb[processesRunning].ctx.sp = (uint32_t) &tos_console + processesRunning*0x00001000;
      pcb[processesRunning].priority = processesRunning+5;
      pcb[processesRunning].initialpriority = processesRunning+5;
      pcb[processesRunning].prioritychange = processesRunning+1;
      uint32_t currentsp = (uint32_t)&tos_console + current->pid*0x00001000;
      memcpy((void *) pcb[processesRunning].ctx.sp - 0x00001000, (void *) currentsp - 0x00001000, 0x00001000);
      pcb[processesRunning].ctx.gpr[0]=0;
      ctx->gpr[0] = processesRunning;
      break;


    }
      case 0x04:{
          print("EXIT");
          current->status = STATUS_TERMINATED;
          schedule(ctx);
          break;
      }

      case 0x05:{
        print("EXEC");
        ctx->pc = ctx->gpr[0];
        ctx->sp = (uint32_t) &tos_console + processesRunning*0x00001000;

        break;
      }

      case 0x06:{
        print("KILL");
        int id  = ctx->gpr[0];
        pcb[id].status = STATUS_TERMINATED;
      }
          
//       case 0x08:{
//           print("CHWRITE");
//           int fd = ctx->gpr[0];
//           int x = ctx->gpr[1];
//           int id = ctx->gpr[2];
//           if (channels[fd].lastwrite == id){
//               ctx->gpr[0]= -1;
//           }
//           else {
//               channels[fd].lastwrite = id;
//               channels[fd].data = x;
//               ctx->gpr[0] = 0;
//           }
//           break;
//       }
//       case 0x09:{
//           print("CHREAD");
//           int fd = ctx->gpr[0];
//           int id = ctx->gpr[1];
//           int x = 0;
//           if (channels[fd].lastwrite == id){
//               x=0;
//           }
//           else {
//              x = channels[fd].data;
//              channels[fd].data = 0;
//           }
//           ctx->gpr[0]=x;
//           break;
//       }
//       case 0x10:{
//           print("PIPE");
//           int fd = ctx->gpr[0];
//           int block = ctx->gpr[1];
//           int process_a = ctx->gpr[2];
//           int process_b = ctx->gpr[3];
//           channels[fd].process_a = process_a;
//           channels[fd].process_b = process_b;
//           channels[fd].data = 0;
//           channels[fd].channelID = fd+1;
//           channels[fd].lastwrite = -1;
//           ctx->gpr[0] = fd;
//           break;
//       }
//       case 0x11:{
//           print("SYSOPEN");
//           int fd = ctx->gpr[0];
//           int id = ctx->gpr[1];
//           if (channels[fd].process_a == 0){
//               channels[fd].process_a = id;
//           }
//           else {
//               channels[fd].process_b = id;
//           }
//           ctx->gpr[0] = fd;
//           break;
//       }
//       case 0x12:{
//           print("SYSCLOSE");
//           int fd = ctx->gpr[0];
//           int id = ctx->gpr[1];
//           if (channels[fd].process_a == id){
//               channels[fd].process_a = 0;
//           }
//           else {
//               channels[fd].process_b = 0;
//           }
//           break;
//       }
//       case 0x13:{
//           print("SYSID");
//           ctx->gpr[0] = currentlyExecuting;
//           break;
//       }
      case 0x08:{
//           int fd = ctx->gpr[0];
//           sem_t *mutex = 0;
      }
      case 0x09:{
          //if not available change to status wait and add to queue
         sem_t* mutex = (sem_t*) ctx->gpr[0];
     if (*mutex ==0){
         current->status = STATUS_WAITING;
         for (int i=0; i<count;i++){
         if (count>0 && (wlist[i].pid == -1)){
             count = i;
         }
         }
         wlist[count].pid=pcb[processesRunning].pid;
         wlist[count].mutex = (sem_t*) mutex;
         count++;  

         
     }
      else if (*mutex==1){
              sem_t* mutex = (sem_t*) ctx->gpr[0];
          *mutex = 0;
      }
           }
      case 0x10:{
          sem_t* mutex = (sem_t*) ctx->gpr[0];
          *mutex = 1;
//           schedule();
          
      }

    default   : { // 0x?? => unknown/unsupported
      break;
    }
  }

  return;
}
