#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

typedef struct mat_t {
   int r;
   int c;
   long long *m;
} mat_t;

mat_t arrayMax[100];
mat_t result[100];
int r1, c1, r2, c2, rchain, cchain, ticket, counter, counter2;


int resultMatrixInit(int nrow, int ncol, int arrayTicket){
    int i, j;
    result[arrayTicket].r = nrow;
    result[arrayTicket].c = ncol;
    result[arrayTicket].m = malloc(nrow*ncol*sizeof(long long));
    for (i = 0; i < nrow; i++){
        for(j = 0; j < ncol; j++){
            result[arrayTicket].m[i*result[arrayTicket].c+j] = 0;
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

int matrixBuilder(int nrow, int ncol, mat_t *m){
    int i, j;
    for(i = 0; i < nrow; i++){
        for(j =0; j < ncol; j++){
            scanf("%lld", &m->m[i*m->c+j]); 
        }
    }
    return 0;
}

int multiplier(mat_t *m1, mat_t *m2, int arrayTicket){
    int i, j, k;

    resultMatrixInit(m1->r, m2->c, arrayTicket);

    for (i = 0; i < m1->r; i++){
        for(j = 0; j < m2->c; j++){
            for(k = 0; k < m1->c; k++){
                result[arrayTicket].m[i*result[arrayTicket].c+j] += m1->m[i*m1->c+k] * m2->m[k*m2->c+j];
            }
        }
    }
}

// Scans two at a time and puts them into arrayMax[100]

void readingInMatrix(){
    ticket = 0;
    counter = 0;
    scanf("%d", &arrayMax[ticket].r);
    scanf("%d", &arrayMax[ticket].c);
    while (arrayMax[ticket].r != 0 && arrayMax[ticket].c != 0){
        matrixInitializer(arrayMax[ticket].r, arrayMax[ticket].c, ticket);
        matrixBuilder(arrayMax[ticket].r, arrayMax[ticket].c, &arrayMax[ticket]);
        ticket++;
        counter++;
        scanf("%d", &arrayMax[ticket].r);
        scanf("%d", &arrayMax[ticket].c);
    }
    printf("Counter = %d\n", counter);
}


void calculateMatrix(){
    ticket = 0;
    counter2 = 0;
    int i;
    for (i = 0; i < counter; i++){
        multiplier(&arrayMax[ticket], &arrayMax[ticket+1], ticket);
        printMatrix(&result[ticket]);
        ticket++;
        counter2++;
    }
}

void printAll(){
    ticket = 0;
    int i;
    printf("counter2 = %d\n", counter2);
    for (i = 0; i < counter2; i++){
        printf("ticket = %d\n", ticket);
        printMatrix(&result[ticket]);
        ticket++;
    }
}

int main(int argc, char *argv[])
{
    int i, j, k;
    ticket = 0;
    readingInMatrix();
    calculateMatrix();
    printAll();
    /* // First matrix
    scanf("%d", &r1); 
    scanf("%d", &c1);

    matrixInitializer(r1, c1, ticket);
    matrixBuilder(arrayMax[ticket].r, arrayMax[ticket].c, &arrayMax[ticket]);

    // Second Matrix
    scanf("%d", &r2); 
    scanf("%d", &c2);

    matrixInitializer(r2, c2, ticket+1);
    matrixBuilder(arrayMax[ticket+1].r, arrayMax[ticket+1].c, &arrayMax[ticket+1]);

    // Printing the first multiplication result
    resultMatrixInit(arrayMax[ticket].r, arrayMax[ticket+1].c);
    multiplier(&arrayMax[ticket], &arrayMax[ticket+1]);
    printMatrix(&result);

    // Start of the chain
    scanf("%d", &rchain); 
    scanf("%d", &cchain);

    while(rchain != 0 && cchain != 0){

        matrixInitializer(result.r, result.c, ticket+1);

        for (i = 0; i < result.r; i++){
            for ( j = 0; j < result.c; j++){
                arrayMax[ticket+1].m[i*arrayMax[ticket+1].c+j] = result.m[i*result.c+j];
            }
        }
        ticket++;
        matrixInitializer(rchain, cchain, ticket+1);
        matrixBuilder(arrayMax[ticket+1].r, arrayMax[ticket+1].c, &arrayMax[ticket+1]);
        //printMatrix(&arrayMax[ticket+1]);
        resultMatrixInit(arrayMax[ticket].r, arrayMax[ticket+1].c);
        multiplier(&arrayMax[ticket], &arrayMax[ticket+1]);
        printMatrix(&result);
        scanf("%d", &rchain); 
        scanf("%d", &cchain);
        printf("rchain = %d || cchain = %d\n", rchain, cchain);
    } */
    return 0;
}
