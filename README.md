## OS Kernel for Cortex-A8 

### About
* This is a simple OS kernel for a [Cortex A-8 processor](https://developer.arm.com/ip-products/processors/cortex-a/cortex-a8) emulated through ***QEMU***.
* Most of the functionality is developed in [hilevel.c](kernel/hilevel.c) with ***C*** and some assembly in [lolevel.s](kernel/lolevel.s).
* This whole kernel is managed by a [console](kernel/console.c) which allows the direct input of commands into the kernel.

### How to Run:

As is, the console only recognises the following commands:
 *
 * a. execute <program name>
 *
 *    This command will use fork to create a new process; the parent
 *    (i.e., the console) will continue as normal, whereas the child
 *    uses exec to replace the process image and thereby execute a
 *    different (named) program.  For example,
 *    
 *    execute P3
 *
 *    would execute the user program named P3.
 *
 * b. terminate <process ID> 
 *
 *    This command uses kill to send a terminate or SIG_TERM signal
 *    to a specific process (identified via the PID provided); this
 *    acts to forcibly terminate the process, vs. say that process
 *    using exit to terminate itself.  For example,
 *  
 *    terminate 3
 *
 *    would terminate the process whose PID is 3.

### Feature points
* The kernel is able to dynamically instantiate processes which can be specified as a file such as [P4.c](kernel/P4.c) which specifies a program to run. The kernel dynamically shifts between the processes through maximising runtime for high priority tasks and adopts queue based approach for managing processes.
* Has the ability to ```Fork```, ```Exec``` and ```Kill``` processes with each proccess given a unique PID.
* I also used the Kernel to develop a Semaphore Shared Memory approach to solving the [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) with variable numbers of Philosophers which can be run by calling the command ```exec philosophers``` from the console.
 


