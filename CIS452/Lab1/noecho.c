#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

int main()
{
    struct termios tp, save;

       printf ("Disabling Terminal Echo\n");

       tcgetattr(1, &tp);
       save = tp;                          /* So we can restore settings later */
       tp.c_lflag &= ~ECHO;                /* ECHO off, other bits unchanged */
       tcsetattr(1,TCSANOW,&tp);

       char* input = malloc (16);
       printf ("Please Enter Secret Word: \n> ");
       scanf ("%s", input);

       printf("Your Secret Word Is: %s\n",input);

       printf("Restoring State.\n");

       tcsetattr(1, TCSANOW, &save);

       printf("Please enter visible word:\n> ");
       scanf("%s", input);

       free (input);

       return 0;
}
