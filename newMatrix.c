#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#define MAXCOUNT 105

typedef struct mat_t {
   int r;
   int c;
   long long *m;
} mat_t;

mat_t result;
mat_t arrayMax[MAXCOUNT];

int ticket       = 0;
int readIndex    = 0; 
int writeIndex   = 2;
int computeIndex = 3;

uint64_t diffr   = 0;
uint64_t diffw   = 0;
uint64_t diffc   = 0;

pthread_mutex_t readMutex    = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writeMutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ticketMutex  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t computeMutex = PTHREAD_MUTEX_INITIALIZER;

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

// Scans two at a time and puts them into arrayMax[MAXCOUNT]

void *readingInMatrix(){
    pthread_mutex_lock(&ticketMutex);
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &arrayMax[ticket].r);
    scanf("%d", &arrayMax[ticket].c);
    while (ticket + readIndex >= ticket && ticket < MAXCOUNT && arrayMax[ticket].r != 0 && arrayMax[ticket].c != 0){
        matrixInitializer(arrayMax[ticket].r, arrayMax[ticket].c, ticket);
        matrixBuilder(arrayMax[ticket].r, arrayMax[ticket].c, &arrayMax[ticket]);
        ticket++;
        readIndex++;
        scanf("%d", &arrayMax[ticket].r);
        scanf("%d", &arrayMax[ticket].c);
    } 
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    pthread_mutex_unlock(&ticketMutex); 
    return NULL;
}


void *calculateMatrix(){
    pthread_mutex_lock(&ticketMutex);
    //printf("\n");
    clock_gettime(CLOCK_REALTIME, &startCompute);
    while(computeIndex < ticket - 1){
        resultMatrixInit(arrayMax[computeIndex].r, arrayMax[computeIndex+1].c);
        multiplier(&arrayMax[computeIndex], &arrayMax[computeIndex+1], computeIndex, arrayMax[computeIndex].r);
        computeIndex++;      
    }
    clock_gettime(CLOCK_REALTIME, &endCompute);
    diffc += 1000000000 * (endCompute.tv_sec - startCompute.tv_sec) + endCompute.tv_nsec - startCompute.tv_nsec;
    pthread_mutex_unlock(&ticketMutex);
    return NULL;
}

void *printAll(){
    pthread_mutex_lock(&ticketMutex);
    clock_gettime(CLOCK_REALTIME, &startWrite);
    while(writeIndex < ticket - 1){
        
        printf("\n#%d Result\n", writeIndex + 1);
        printf("--------------\n");
        printMatrix(&arrayMax[writeIndex+1]);
        writeIndex++;     
    }
    clock_gettime(CLOCK_REALTIME, &endWrite);
    diffw += 1000000000 * (endWrite.tv_sec - startWrite.tv_sec) + endWrite.tv_nsec - startWrite.tv_nsec;
    pthread_mutex_unlock(&ticketMutex);
    return NULL;
}

void *printTime(void *value){

    printf("-------TIMER-------\n");
    fprintf(stderr, "Reading:   %luns\n", diffr);
    fprintf(stderr, "Computing: %luns\n", diffc);
    fprintf(stderr, "Writing:   %luns\n", diffw);

    return NULL;
}

void *testThread(void *value){
    printf("TEST TEST TEST\n");
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
        //pthread_create(&time, NULL, printTime, NULL);

        pthread_join(read, NULL);
        pthread_join(compute, NULL);
        pthread_join(write, NULL);
        //pthread_join(time, NULL);
        printTime(NULL);

        exit(0);
    } 
    else {
        printf(">>> Single-threaded <<<\n");
        ticket = 0;
        readingInMatrix(NULL);
        calculateMatrix(NULL);
        printAll(NULL);
        printTime(NULL);
        return 0;
    }
}
