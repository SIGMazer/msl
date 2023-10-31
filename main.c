
#define MSL_DS_IMPLEMENTATION
#include "msl_ds.h"



#include <stdio.h>

typedef struct{
    char *key;
    char *value;
} KeyVal;

bool token_eq(const void *a, const void *b){
    a = (char *)a;
    b = (char *)b;
    if(strlen(a) != strlen(b)) return false;
    else{
       return memcmp(a, b, strlen(a)) == 0;
    }
}

int main(int argc, char *argv[])
{

    KeyVal *kv = NULL;
    hmput(kv, "hello", "world", token_eq);
    printf("%s\n", (char*)hmget(kv, "hello", token_eq));


    return EXIT_SUCCESS;
}
