#include "headers.h"
#include "misc.h"

void red(){
    fprintf(stderr, "\033[31;1m");
}

void green(){
    printf("\033[32;1m");
}

void yellow(){
    printf("\033[33;1m");
}

void blue(){
    printf("\033[34;1m");
}

void reset(){
    printf("\033[0;0m");
}

void reset_error(){
    fprintf(stderr, "\033[0;0m");
}

void error(char *s){
    red();
    fprintf(stderr, s);
    reset_error();
}