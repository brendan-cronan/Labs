#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>



#define SIZE 4096


char* shmPtr;
int shmId;

void sigHandler(int sigNum);


int main ()
{


  signal (SIGINT, sigHandler);

   struct shmid_ds buf;


   key_t key = ftok("token.txt", 'a');
   printf("Key is: %d\n",key);


   if ((shmId = shmget (key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("Unable to get shmID...\n");
     exit (1);
   }

   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach to segment.\n");
      exit (1);
   }
   printf("ShmID: %d\n",shmId);


   //wrtFlag
   shmPtr[0]='0';
   //read1Flag
   shmPtr[1]='0';
   //read2Flag
   shmPtr[2]='0';



   char* pointer;
   char wrtFlag,readFlag;

   char* input;
   input = malloc(SIZE);

   while(1){

     printf("Please Input String to send to Reader...\n  >  ");
     fgets(input,100,stdin);
     //scanf ("%s", input);
     if (*input == 'q'){
       sigHandler(SIGINT);
     }

     //shmPtr is our address to work with.
     //3 flags for deciding when to read/write.
     // wrtFlag = 1 if the writer has finished writing the string to segment.
     // read1Flag = 1 if the 1st reader has finished reading the string.
     // read2Flag = 1 if the 2nd reader has finished reading the string.
     // wrtFlag gets set to 0 iff readFlag is true and the other then sets wrtFlag.
     // FALSE = 0 ... TRUE = any other value.



     //infinite loop while waiting on readers.
     while(shmPtr[0]=='1'){
       sleep(1);
     }
     //now, the readers have both read the string.

     //after 3 initial flags, start of string pointer.
     char* strPtr = &shmPtr[3];

     for(int i = 0; input[i] != '\n'; i++) {

       *strPtr++ = input[i];

     }
     *strPtr++ = '\n';


     shmPtr[0] = '1';//writer is done.

   }


   return 0;
}

void sigHandler(int sigNum){
  sleep(1);
  printf("Writer is quitting...");

  if (shmdt (shmPtr) < 0) {
     perror ("Cannot Detach from Segment.\n");
     exit (1);
  }

  if (shmctl (shmId, IPC_RMID, 0) < 0) {
     perror ("can't deallocate\n");
     exit(1);
  }



  exit(0);
}
