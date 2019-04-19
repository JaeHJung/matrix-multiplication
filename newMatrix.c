#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#define MAXCOUNT 105

typedef struct mat_t {
   int r;
   int c;
   long long *m;
} mat_t;

mat_t result;
mat_t arrayMax[MAXCOUNT];

int readIndex    = 0; 
int writeIndex   = 0;
int computeIndex = 0;
int arrayCount   = 0;


uint64_t diffr   = 0;
uint64_t diffw   = 0;
uint64_t diffc   = 0;

pthread_mutex_t ticketMutex  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t event  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t compute  = PTHREAD_MUTEX_INITIALIZER;

struct timespec startRead, endRead, startCompute, endCompute, startWrite, endWrite;

int resultMatrixInit(int nrow, int ncol){
    int i, j;
    result.r = nrow;
    result.c = ncol;
    result.m = malloc(nrow*ncol*sizeof(long long));
    for (i = 0; i < nrow; i++){
        for(j = 0; j < ncol; j++){
            result.m[i*result.c+j] = 0;
        }
    }

    return 0;
}

int matrixInitializer(int nrow, int ncol, int arrayTicket){

    int i, j;
    arrayMax[arrayTicket].r = nrow;
    arrayMax[arrayTicket].c = ncol;
    arrayMax[arrayTicket].m = malloc(nrow*ncol*sizeof(long long));

    for (i = 0; i < nrow; i++){
        for(j = 0; j < ncol; j++){
            arrayMax[arrayTicket].m[i*arrayMax[arrayTicket].c+j] = 0;
        }
    }

    return 0;
}

int matrixBuilder(int nrow, int ncol, mat_t *m){

    int i, j;
    for(i = 0; i < nrow; i++){
        for(j =0; j < ncol; j++){
            scanf("%lld", &m->m[i*m->c+j]); 
        }
    }

    return 0;
}

int multiplier(mat_t *m1, mat_t *m2, int arrayTicket, int defaultRow){

    int i, j, k, offset;
    offset = arrayTicket + 1;
    for (i = 0; i < defaultRow; i++){
        for(j = 0; j < m2->c; j++){
            for(k = 0; k < m1->c; k++){
                result.m[i*result.c+j] += m1->m[i*m1->c+k] * m2->m[k*m2->c+j];
            }
        }
    }

    matrixInitializer(defaultRow, result.c, offset);

    for (i = 0; i < defaultRow; i++){
        for(j = 0; j < result.c; j++){
            arrayMax[offset].m[i*arrayMax[offset].c+j] += result.m[i*result.c+j];
        }
    }
    return 0;
}

int printMatrix(mat_t *m){

    int i, j;
    for (i=0; i<m->r; i++) {
        for (j=0; j<m->c; j++){
            printf("%lld ", m->m[i*m->c+j]);
        }
        printf("\n");
    }

    return 0;
}

void *readingInMatrix(){
    clock_gettime(CLOCK_REALTIME, &startRead);
    pthread_mutex_lock(&event);
    while ((scanf("%d", &arrayMax[readIndex].r)!= EOF && scanf("%d", &arrayMax[readIndex].c)!= EOF)){
        pthread_mutex_unlock(&event);
        if(arrayMax[readIndex].r == 0 && arrayMax[readIndex].c == 0){
            break;
        }
        matrixInitializer(arrayMax[readIndex].r, arrayMax[readIndex].c, readIndex);
        matrixBuilder(arrayMax[readIndex].r, arrayMax[readIndex].c, &arrayMax[readIndex]);
        readIndex++;
        pthread_mutex_lock(&ticketMutex);
        arrayCount++;
        pthread_mutex_unlock(&ticketMutex);
        pthread_mutex_lock(&event);
    }
    pthread_mutex_unlock(&event);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    return NULL;
}


void *calculateMatrix(){
    clock_gettime(CLOCK_REALTIME, &startCompute);
    pthread_mutex_lock(&event);
    while((arrayMax[computeIndex].r != 0 && arrayMax[computeIndex].c != 0) || (arrayMax[computeIndex+1].r != 0 && arrayMax[computeIndex+1].c != 0)){
        pthread_mutex_unlock(&event);
        pthread_mutex_lock(&ticketMutex);
        if ((arrayCount - computeIndex) >= 1){
            pthread_mutex_unlock(&ticketMutex);
            resultMatrixInit(arrayMax[computeIndex].r, arrayMax[computeIndex+1].c);
            multiplier(&arrayMax[computeIndex], &arrayMax[computeIndex+1], computeIndex, arrayMax[computeIndex].r);
            pthread_mutex_lock(&compute);
            computeIndex++;
            pthread_mutex_unlock(&compute);
        } else {
            pthread_mutex_unlock(&ticketMutex);
        }
        pthread_mutex_lock(&event);
    }
    pthread_mutex_unlock(&event);
    clock_gettime(CLOCK_REALTIME, &endCompute);
    diffc += 1000000000 * (endCompute.tv_sec - startCompute.tv_sec) + endCompute.tv_nsec - startCompute.tv_nsec;
    return NULL;
}

void *printAll(){
    
    clock_gettime(CLOCK_REALTIME, &startWrite);
    pthread_mutex_lock(&event);
    while(arrayMax[writeIndex].r != 0 && arrayMax[writeIndex].c != 0){
        pthread_mutex_unlock(&event);
        pthread_mutex_lock(&compute);
        if((computeIndex - writeIndex) >= 1){
            pthread_mutex_unlock(&compute);
            printMatrix(&arrayMax[writeIndex+1]);
            writeIndex++;
        } else {
            pthread_mutex_unlock(&compute);
        }
        pthread_mutex_lock(&event);
    }
    pthread_mutex_unlock(&event);
    clock_gettime(CLOCK_REALTIME, &endWrite);
    diffw += 1000000000 * (endWrite.tv_sec - startWrite.tv_sec) + endWrite.tv_nsec - startWrite.tv_nsec;
    
    return NULL;
}

void *printTime(void *value){

    fprintf(stderr, "Reading:   %luns\n", diffr);
    fprintf(stderr, "Computing: %luns\n", diffc);
    fprintf(stderr, "Writing:   %luns\n", diffw);

    return NULL;
}

int main(int argc, char *argv[])
{
    printf("Begin Matrix Multiplication\n");
    if (argc > 1) {
        printf(">>> Multithreading <<<\n");
        pthread_t read, compute, write;

        pthread_create(&read, NULL, readingInMatrix, NULL);
        pthread_create(&compute, NULL, calculateMatrix, NULL);
        pthread_create(&write, NULL, printAll, NULL);

        pthread_join(read, NULL);
        pthread_join(compute, NULL);
        pthread_join(write, NULL);
        printTime(NULL);

        exit(0);
    } 
    else {
        printf(">>> Single-threaded <<<\n");
        //ticket = 0;
        readingInMatrix(NULL);
        calculateMatrix(NULL);
        printAll(NULL);
        printTime(NULL);
        return 0;
    }
}
