#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FALSE 0
#define TRUE !(FALSE)

int makeLink(char* stuff[], int sym_flag){
  const char* TargetFile = *stuff;
  const char* Destination = *(stuff+1);

  if(sym_flag == 1){//Hard link
    return link(TargetFile, Destination);
  }
  else{
    return symlink(TargetFile, Destination);
  }


}




int main(int argc, char *argv[])
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;

   if (argc < 3) {
      printf ("Usage: ln -(h/s) TargetFile Destination.\n");
      exit(1);
   }

   int sym_flag= FALSE;//Default to hard link(false)

   if(strcmp(&argv[1][0], "-")){//flag is present.
     if(strcmp(&argv[1][1], "h")){
       sym_flag= FALSE;
     }

     else if (strcmp( &argv[1][1], "s")) {
       sym_flag = TRUE;
     }
     if(makeLink(&argv[2],sym_flag) < 0){
       printf("ERROR:  Hard symbolic is not valid");
     }
   }
   else{//flag is not present.
     sym_flag=FALSE;
     if(makeLink(&argv[1],sym_flag) < 0){
       printf("ERROR:  Hard link is not valid");
     }
   }



   // dirPtr = opendir (argv[1]);

   // printf ("Name of File:\t\t");
   // printf ("File Size(bytes):\t");
   // printf ("UserID:\t");
   // printf ("GroupID:\t");
   // printf ("Inode#:\t");
   //
   //
   // printf ("\n");

   //
   // while ((entryPtr = readdir (dirPtr))){
   //   char* fileName = entryPtr->d_name;
   //
   //   stat (fileName, &statBuf);
   //
   //   off_t size = statBuf.st_size;
   //   uid_t uid = statBuf.st_uid;
   //   uid_t userId = statBuf.st_uid;
   //   gid_t groupId = statBuf.st_gid;
   //   ino_t inode = statBuf.st_ino;
   //
   //   printf ("%-20s\t",fileName);
   //   printf ("%lld\t\t\t",size);
   //   printf ("%u\t",userId);
   //   printf ("%u\t\t",groupId);
   //   printf ("%llu\t",inode);
   //
   //
   //   printf ("\n");
   //
   // }
   //
   // closedir (dirPtr);
   return 0;
}
