#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>


int main ()
{
  sem_t* sem;
  sem_init(&sem, 0, 0);
  int ct = 0;

  for(;;){
    if(sem_post(&sem) < 0){
      printf("the maximum semaphore value is: %d",ct);
      sem_destroy(&sem);
      exit(1);
    }
    else
      ct++;
  }

  return 0;
}
