#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main ()
{
  int shmid;
  char * shmPtr;

  int memsize = 4096;
  for(;;){
    memsize = memsize + 1024;
    if( (shmid = shmget(IPC_PRIVATE, memsize, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
      perror("Too much memory.\n");
      printf("%d is the maximum memory size.\n",memsize-1024);
      exit(1);
    }

    if((shmPtr = shmat(shmid, 0,0) ) == (void*) -1     ){
      perror("Can't Attach\n");
      exit(1);
    }

    if( shmdt(shmPtr) < 0  ){
      perror("Don't Let Go!\n");
      exit(1);
    }

    if(shmctl(shmid, IPC_RMID, 0) < 0){
      perror("Unable to deallocate memory.\n");
      exit(1);
    }
  }
  return 0;
}
