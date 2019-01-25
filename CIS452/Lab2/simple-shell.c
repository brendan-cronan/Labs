#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main (int argc, char* argv[])
{
  while (1){
    int childPid=0;



    char cwd[100];
    getcwd(cwd, 100);
    printf("\n-----%s-----\n", cwd);
    printf("> ");


    char line[100];
    fgets(line, 1000000, stdin);
    int len = strlen(line);
    if( line[len-1]=='\n')
      line[len-1]='\0';
    printf("Original Input: %s\n\n", line);


    if(line[0] == 'q'){
      printf("Quitting\n");
      exit(0);
    }

    int bufsize = 64;
    int position = 0;
    char *tokens[bufsize];
    char *token;

    token = strtok(line, " ");
    while (token != NULL) {
      tokens[position] = token;
      position++;

      token = strtok(NULL, " ");
    }
    tokens[position] = NULL;







      pid_t pid;
      printf("Forking\n" );
      pid = fork();

      if( pid < 0 )
        exit(1);
      else if( pid == 0 ){//Child Process
        printf("CHILD PID: %ld \n",(long)getpid());

        if (execvp(tokens[0],tokens) < 0) {
          perror("ERROR: ");
          exit(1);
        }
        exit(0);

      }
      else {
        printf("PARENT PID: %ld \n",(long)getpid());
        int status;
        struct rusage usage;
        getrusage(RUSAGE_CHILDREN, &usage);

        struct timeval begin, end;
        double ctx_before, ctx_after;

        ctx_before = usage.ru_nivcsw;
        begin = usage.ru_utime;

        pid_t child = wait(&status);

        getrusage(RUSAGE_CHILDREN, &usage);
        end = usage.ru_utime;
        ctx_after= usage.ru_nivcsw;

        printf("Child: %d returned with status: %d\n",child, status);

        long cpu_sec, cpu_microsec;
        cpu_sec = end.tv_sec - begin.tv_sec;
        cpu_microsec = end.tv_usec - begin.tv_usec;



        printf("CPU Usage in Seconds: %ld, %ld Microseconds.\n",cpu_sec,cpu_microsec);
        printf("Total Context switches = %d\n",(int)(ctx_after-ctx_before));

      }


  }
  return 0;
}
