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
mat_t result;
int r1, c1, r2, c2, rchain, cchain, ticket, counter, counter2;


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
}


void calculateMatrix(){
    ticket = 0;
    int i, anchorRow;
    anchorRow = arrayMax[ticket].r;
    for (i = 0; i < counter-1; i++){
        resultMatrixInit(anchorRow, arrayMax[ticket+1].c);
        /* printMatrix(&result[ticket]); */
        multiplier(&arrayMax[ticket], &arrayMax[ticket+1], ticket, anchorRow);
        ticket++;
    }
}

void printAll(){
    ticket = 1;
    int i;
    for (i = 0; i < counter-1; i++){
        printf("\n#%d Result\n", i+1);
        printf("--------------\n");
        printMatrix(&arrayMax[ticket]);
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
    return 0;
}
