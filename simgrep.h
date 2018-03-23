#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <string.h>

#define OP_TRUE 1
#define OP_FALSE 0
#define NON_EXISTENT -1

typedef struct
{
    int ignore;  /* Value for the "-i" optional argument. */
    int file_name;  /* Value for the "-l" optional argument. */
    int line_number;  /* Value for the "-n" optional argument. */
    int count;  /* Value for the "-c" optional argument. */
    int word;  /* Value for the "-w" optional argument. */
    int recursive;  /* Value for the "-r" optional argument. */

    int pattern_pos;
    int file_dir_pos;
} option;

int argchk(int argc, char* argv[], option* op);
char** file_search(char* pattern, option* op);
void printRes(char** res);
