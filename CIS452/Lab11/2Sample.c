#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat statBuf;


   dirPtr = opendir (".");

   printf ("Name of File:\t\t\tFile Size(bytes):\n");

   while ((entryPtr = readdir (dirPtr))){
     char* fileName = entryPtr->d_name;

     stat (fileName, &statBuf);

     off_t size = statBuf.st_size;

     printf ("%-20s\t\t%lld\n", fileName,size);
   }

   closedir (dirPtr);
   return 0;
}
