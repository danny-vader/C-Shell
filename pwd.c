#include "headers.h"
#include "pwd.h"

extern char CWD[MAX_SIZE];

void pwd(){
    printf("%s\n", CWD);
}