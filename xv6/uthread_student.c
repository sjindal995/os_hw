#include "uthread.h"

void 
thread_schedule(void)
{
  thread_p t;
  int i;


  /*HW: 
      Read the schedular code below to understand what it is doing and the order 
        in which it schedules the threads.
   
    */

    /* To be improved and implemented in the HW: Change the schedular to implement the below behavior:
       The highest priority thread has to be scheduled first (priority=1 is higher than priority =2).
    	 If there are more than one thread with same priority, then they need to be scheduled in round-robin manner.

	 	 stop search when all threads are checked and none can be scheduled.
    */

  /* Find another runnable thread. */
    t=current_thread+1;
    int min_p = -1;
    for( i=0;i<MAX_THREAD;i++){
      if((all_thread+i)->state == RUNNABLE){
        min_p = (all_thread+i)->priority;
        break;
      }
    }  
  for( i=0;i<MAX_THREAD;i++){
    if((all_thread+i)->priority < min_p && (all_thread+i)->state == RUNNABLE) min_p = (all_thread+i)->priority;
  }
  for (i=0;i<MAX_THREAD;i++) {
  	if (t >= all_thread+MAX_THREAD) t = all_thread;
    if (t->state == RUNNABLE && t->priority == min_p) {
        next_thread = t;
        break;
    }
    t = (t+1);
  }


  if (t >= all_thread + MAX_THREAD && current_thread->state == RUNNABLE) {
    /* The current thread is the only runnable thread; run it. */
    next_thread = current_thread;
  }

  if (next_thread == 0) {
    printf(2, "thread_schedule: no runnable threads; deadlock\n");
    exit();
  }

  if (current_thread != next_thread) {         /* switch threads?  */
    next_thread->state = RUNNING;
    thread_switch();
  } else
    next_thread = 0;
}

void 
thread_create(void (*func)(), int priority)
{
  thread_p t;

  //HW: Your code here
  //starting from all_thread, scan and find out which slot is FREE
  //use that slot to create a new thread
  int i;
  for(i = 0; i<MAX_THREAD;i++){
    t = &all_thread[i];
    if(t->state == FREE) break;
  }
  if(t->state != FREE){
    printf(2,"no free slot for thread_create\n");
    exit();
  }

  // set esp to the top of the stack
  // leave space for return address
  // push return address on stack
  // leave space for registers that thread_switch will push
  t->sp = (int)(t->stack + STACK_SIZE);
  // leave space for return address
  t->sp -= 4;
  //push return address on the stack;
  *((int *)t->sp) = (int)func;
  // space for registers EAX,EBX,ECX,EDX,ESP,EBP,ESI,EDI
  t->sp -= 32;
  t->priority = priority;
  t->state = RUNNABLE;
}

