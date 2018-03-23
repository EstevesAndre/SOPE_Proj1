#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <string.h>
#include <fcntl.h>
#include "simgrep.h"

int main(int argc, char *argv[])
{
    option op = {OP_FALSE, OP_FALSE, OP_FALSE, OP_FALSE, OP_FALSE, OP_FALSE, NON_EXISTENT, NON_EXISTENT};
    int r = argchk(argc, argv, &op);
    if(r != 0)
      return r;   

    if(op.file_dir_pos != NON_EXISTENT) //there is a file/dir specified
    {
       struct stat s;
       stat(argv[op.file_dir_pos], &s);
       if(S_ISREG(s.st_mode))
       {
          int fd = open(argv[op.file_dir_pos], O_RDONLY);
          if(fd == -1)
          {
             printf("failed to open: %s\n", argv[op.file_dir_pos]);
             return 2;
          }
          dup2(fd, STDIN_FILENO); 
       }
       else if(S_ISDIR(s.st_mode))
       {

       }
       else
       {
           printf("file or directory not found: %s\n", argv[op.file_dir_pos]);
           return 3;
       }
     }

     /////PROVISÓRIO - APENAS PARA TESTE
     char** res = file_search(argv[op.pattern_pos], &op);
     printRes(res);
     /////
    return 0; 
} 

char** file_search(char* pattern, option* op)
{
   FILE* f = fdopen(STDIN_FILENO, "r");
   int t = 1024;
   int ret_pos = 0;
   char* line = malloc((t+1) * sizeof(char));

   fseek(f, 0, SEEK_END);
   int line_no = ftell(f);
   if(line_no == -1)
     line_no = 100; 

   char** ret = malloc( line_no * sizeof(char));
   fseek(f, 0, SEEK_SET);
   line = fgets(line, t, f);
   while(line != NULL)
   {
       if(strstr(line, pattern) != NULL)
       {
           ret[ret_pos] = malloc((t+1)*sizeof(char));
	   strcpy(ret[ret_pos], line);
           ret_pos++;
       }
       line = fgets(line, t, f);
   }
   ret[ret_pos] = NULL;

   free(line);
   return ret;
}

void printRes(char** res)
{
   int i = 0;

   while(res[i] != NULL)
   {
      printf("%s", res[i]);
      i++;
   }
}

int argchk(int argc, char* argv[], option* op)
{
    if (argc == 1)
    {
	printf("usage: %s [options] pattern [file/dir]\n", argv[0]);
        return 1;
    }

    int i;
    for (i = 1; i < argc; i++)
    {	
        if (strcmp(argv[i], "-i") == 0) /* Option "-i" entered */
        {
            op->ignore = OP_TRUE;
        }
        else if (strcmp(argv[i], "-l") == 0) /* Option "-l" entered */
        {
            op->file_name = OP_TRUE;
        }
        else if (strcmp(argv[i], "-n") == 0) /* Option "-n" entered */
        {
            op->line_number = OP_TRUE;
        }
        else if (strcmp(argv[i], "-c") == 0) /* Option "-c" entered */
        {
            op->count = OP_TRUE;
        }
        else if (strcmp(argv[i], "-w") == 0) /* Option "-w" entered */
        {
            op->word = OP_TRUE;
        }
        else if (strcmp(argv[i], "-r") == 0) /* Option "-r" entered */
        {
            op->recursive = OP_TRUE;
        }
        else
        {
            if(i == (argc - 1)) /* If pattern is last argument (no file/dir) */
            {
                op->pattern_pos = i;
                break;
            }
            else if (i == (argc - 2)) /* If pattern is second-to-last argument (file/dir entered) */
            {
                op->pattern_pos = i;
                op->file_dir_pos = i+1;
                break;
            }
            else /* other input */
            {
                printf("usage: %s [options] pattern [file/dir]\n", argv[0]);
                return 1;
            }
        }
    }

    if(op->pattern_pos == NON_EXISTENT)
    {
       printf("usage: %s [options] pattern [file/dir]\n", argv[0]);
       return 1;
    }
    return 0;
}
