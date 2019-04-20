#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;

   if (argc < 2) {
      printf ("Usage: filename required\n");
      exit(1);
   }



   dirPtr = opendir (argv[1]);

   printf ("Name of File:\t\t");
   printf ("File Size(bytes):\t");
   printf ("UserID:\t");
   printf ("GroupID:\t");
   printf ("Inode#:\t");


   printf ("\n");


   while ((entryPtr = readdir (dirPtr))){
     char* fileName = entryPtr->d_name;

     stat (fileName, &statBuf);

     off_t size = statBuf.st_size;
     uid_t uid = statBuf.st_uid;
     uid_t userId = statBuf.st_uid;
     gid_t groupId = statBuf.st_gid;
     ino_t inode = statBuf.st_ino;

     printf ("%-20s\t",fileName);
     printf ("%lld\t\t\t",size);
     printf ("%u\t",userId);
     printf ("%u\t\t",groupId);
     printf ("%llu\t",inode);


     printf ("\n");

   }

   closedir (dirPtr);
   return 0;
}
