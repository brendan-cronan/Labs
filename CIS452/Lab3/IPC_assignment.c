#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void sigHandler (int);

int main()
{

    pid_t pid, parent_pid;
    parent_pid= getpid();

    if ((pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    }

   else if (pid > 0 ) {// if pid is NOT zero.  AKA: Parent.
     printf("PARENT PROCESS PID = %d\n",getpid());

     signal (SIGUSR1, sigHandler);
     signal (SIGUSR2, sigHandler);
     signal (SIGINT, sigHandler);

     while(1){

       printf ("\nwaiting...\n");
       pause();

     }



    }
    else{
      //CHILD PROCESS
      sleep(1);
      printf("CHILD PROCESS PID = %d\n",getpid());
      while(1){
        int rnd = random()%2;
        int signum;
        if(rnd == 0)
          signum = SIGUSR1;
        else
          signum = SIGUSR2;

        int wait_time = (random()%5)+1;
        //printf("waiting %d\n",wait_time);
        sleep(wait_time);
        kill(parent_pid,signum);

      }
    }


    return 0;
}

void sigHandler (int sigNum)
{
    //printf (" received an interrupt with signal number: %d\n",sigNum);
    if (sigNum == SIGUSR1){
      printf("received a SIGUSR1 signal(%d)\n",sigNum);
    }
    else if (sigNum == SIGUSR2){
      printf("received a SIGUSR2 signal(%d)\n",sigNum);
    }
    else if (sigNum == SIGINT){
      printf(" received an Interrupt signal.(%d)\nGuess I get to see what the process afterlife is like...\n",sigNum);

      sleep (1);
      exit(0);
    }
}
