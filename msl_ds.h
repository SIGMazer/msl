#ifndef MSL_DS_H
#define MSL_DS_H

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    size_t count;
    size_t capacity;
    void *items;
} msl_array_t;


#ifdef MSL_DS_IMPLEMENTATION


#define msl_array_header(array) ((msl_array_t *)array - 1)
#define daput(array,item) msl_array_append(array,item)
#define daputs(array,item,n) msl_array_append_many(array,item,n)
#define dalen(a)  ((a) ? msl_array_header(a)->count : 0)
#define dacap(a)  ((a) ? msl_array_header(a)->capacity : 0)
#define daempty(a) ((a) ? msl_array_header(a)->count = 0 : 0)
#define dafree(a) msl_array_free(a)
#define dadel(a,i) msl_array_del(a,i)
#define dadeln(a,i,n) msl_array_deln(a,i,n)
#define dalast(a) ((a) + msl_array_header(a)->count - 1)
#define dapush daput 
#define dapushs daputs
#define dapop(a) ((a) + --msl_array_header(a)->count)


#define msl_array_append(array,item) \
    (msl_arraymybegrow(array,1), \
    (array)[msl_array_header(array)->count++] = (item))
#define msl_array_append_many(array,items,n) \
    (msl_arraymybegrow(array,n), \
    memcpy((array) + msl_array_header(array)->count, items, (n) * sizeof(*(array))), \
    msl_array_header(array)->count += (n))

#define msl_array_free(a) ((a) ? (free(msl_array_header(a)), (a) = NULL) : 0)

#define msl_array_del(a,i) msl_array_deln(a,i,1) 
#define msl_array_deln(a,i,n) (memmove((a) + (i), (a) + (i) + (n), (msl_array_header(a)->count - (i) - (n)) * sizeof(*(a))), \
    msl_array_header(a)->count -= (n))

#define msl_arraymybegrow(a,n) (!(a) || msl_array_header(a)->count + (n) > msl_array_header(a)->capacity \
                                ? msl_arraygrow(a,n,0) : 0)

#define msl_arraygrow(a,b,c)  ((a) = msl_arraygrowfunc((a), sizeof(*(a)), (b), (c)))



void *msl_arraygrowfunc(void *a, size_t itemlen, size_t addlen, size_t min_cap){

    msl_array_t tmp = {0};
    void *b ;
    size_t min_len = dalen(a) + addlen;
    (void) sizeof(tmp);
    if(min_len > min_cap){
        min_cap = min_len;
    }
    if(min_cap < 2 * dacap(a)){
        min_cap = 2 * dacap(a);
    }
    else if(min_cap < 4){
        min_cap = 4;
    }
    b = realloc(a ? msl_array_header(a) : 0, itemlen * min_cap + sizeof(msla_array_t));
    b = (char *)b + sizeof(msl_array_t);
    if(a == NULL){
        msl_array_header(b)->count = 0;
    }
    msl_array_header(b)->capacity = min_cap;
    return b;
}


#define hmput msl_hmput
#define hmget msl_hmget



#define msl_hmput(hm,key,value, eq) msl_hmput_(hm,key,value,sizeof(value), eq)
#define msl_hmget(hm,key,eq) msl_hmget_(hm,key,eq)




void *msl_hmget_(void *hm, const void *key, bool (*eq)(const void *, const void *)){
    uint32_t hash = djb2(key, strlen(key)) % dacap(hm);
    for(size_t i = 0; i <dacap(hm) && !eq(hm.key,key); i++){
        hash = (hash + 1) % dacap(hm);
    }
    if(!eq(hm.key,key)){
        return hm.value;
    }
    else{
        return NULL;
    }
}

uint32_t djb2(uint8_t buf, size_t len){
    uint32_t hash = 5381;
    for(size_t i = 0; i < len; i++){
        hash = ((hash << 5) + hash) + (uint32_t)buf[i];
    }
    return hash;
}
void msl_hmput_(void *hm, void *key, void *value, size_t value_len, bool (*eq)(const void *, const void *)){
    //init hm
    if(hm == NULL){
        hm = msl_arraygrowfunc(NULL, sizeof(void *), 32, 0);
        msl_array_header(hm)->count = 0;
    }
    // if count > capacity * 0.75 grow
    if(msl_array_header(hm)->count > 0.75 * msl_array_header(hm)->capacity){
        hm = msl_arraygrowfunc(hm, sizeof(void *), 32, 0);
    }

    uint32_t hash = djb2(key, strlen(key)) % dacap(hm);
    for(size_t i = 0; i <dacap(hm) && !eq(hm.key,key); i++){
        hash = (hash + 1) % dacap(hm);
    }
    if(!eq(hm.key,key)){
        hm.key = key;
        hm.value = value;
    }
    else{
        hm.value = value;
    }

}






#endif //MSL_DS_IMPLEMENTATION
#endif //MSL_DS_H

