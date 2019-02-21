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
void sigHandler (int);



int main ()
{

  signal (SIGINT, sigHandler);

   struct shmid_ds buf;


   key_t key = ftok("token.txt", 'a');
   printf("Key Is: %d\n",key);


   if ((shmId = shmget (key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("Unable to get shmID...\n");
     exit (1);
   }

   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach to segment.\n");
      exit (1);
   }
   printf("ShmId: %d\n",shmId);


   //wrtFlag
   shmPtr[0]='0';
   //read1Flag
   shmPtr[1]='0';
   //read2Flag
   shmPtr[2]='0';
   char myFlag = '0';
   while(1){


      //shmPtr is our address to work with.
      //3 flags for deciding when to read/write.
      // wrtFlag = 1 if the writer has finished writing the string to segment.
      // wrtFlag = 0 if the writer has permission to write.
      // read1Flag = 1 if the 1st reader has finished reading the string.
      // wrtFlag gets set to 0 iff readFlag is true and the other then sets wrtFlag.
      // FALSE = 0 ... TRUE = any other value.



      //infinite loop while writer has not yet written to the segment.
      //OR I have already Gone.

      //read1Flag == 1 && read2Flag == 1 -- both have gone -- eq. to wrtFlag = 1
      //read1Flag == 1 && read2Flag == 0 -- one has gone.
      //read1Flag == 0 && read2Flag == 0 -- Neither have gone.
      //my flag indicates that I already went.

      while(shmPtr[0]!='1' || myFlag=='1' ){
        if(shmPtr[2]=='1')
          myFlag='0';
        else
          sleep(1);
      }
      //at this point, writer has written and the readers have a green light.

      //beginning of the string section of the shared memory.
      char* strPtr = &shmPtr[3];

      printf("Reading Value...\n");
      for (char* s = strPtr; *s != '\n'; s++){
        putchar(*s);
      }
      putchar('\n');


      if(shmPtr[1]== '1' ){//if other reader has already completed.
        shmPtr[2] = '1';//Indicate that I have completed, therefore, the other can reset.
        shmPtr[0]='0';//writer is free to over-write the string now.
        //mark both as ready.
        shmPtr[1]='0';


      }
      else if(shmPtr[1] == '0'){//no reader has yet finished.
        shmPtr[1]='1';// indicate that I have completed.
        myFlag='1';
        sleep(2);//wait for 2 seconds.
      }






      /*


      //set flag to indicate completion.
      if(shmPtr[0]=='1' ){//other reader is already done.
        shmPtr[0]='0';//writer is free to over-write the string now.
        shmPtr[1]='0';//reset flag for readers as well.
      }
      else{//other reader is not done.
        shmPtr[1]='1';//indicate that I am done.
        sleep(2);//give it time to complete.
      }

      */







   }





   return 0;
}


void sigHandler (int sigNum){

  sleep(1);
  printf("Reader signing off...");


   if (shmdt (shmPtr) < 0) {
      perror ("Cannot Detach from Segment.\n");
      exit (1);
   }

   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }



}
