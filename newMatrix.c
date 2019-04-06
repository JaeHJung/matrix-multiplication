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
mat_t temp;
mat_t result;

int resultMatrixInit(int nrow, int ncol){
    int i, j;
    result.r = nrow;
    result.c = ncol;
    result.m = malloc(nrow*ncol*sizeof(uint64_t));
    return 0;
}

int matrixInitializer(int nrow, int ncol, int arrayTicket){
    int i, j;
    arrayMax[arrayTicket].r = nrow;
    arrayMax[arrayTicket].c = ncol;
    arrayMax[arrayTicket].m = malloc(nrow*ncol*sizeof(uint64_t));

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

int matrixMultiplier(int r1, int c1, int r2, int c2, uint64_t mtrx1[r1][c1], uint64_t mtrx2[r2][c2], uint64_t tmpMatrix[r1][c2]){
    int i, j, k;

    // Initialzing the temp matrix
    for (i = 0; i < r1; i++){
        for (j = 0; j < c2; j++){
            tmpMatrix[i][j] = 0;
        }
    }
    
    for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            for(k = 0; k < c1; k++){
                tmpMatrix[i][j] += mtrx1[i][k] * mtrx2[k][j];
            }
        }
    }
    return 0;
}

int multiplier(mat_t *m1, mat_t *m2){
    int i, j, k;
    resultMatrixInit(m1->r, m2->c);

    for (i = 0; i < m1->r; i++){
        for(j = 0; j < m2->c; j++){
            for(k = 0; k < m1->c; k++){
                
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int r1, c1, r2, c2, rchain, cchain, ticket, firstTicket;
    ticket = 0;
    firstTicket = ticket;

    scanf("%d", &r1);
    scanf("%d", &c1);
    matrixInitializer(r1, c1, ticket);
    matrixBuilder(arrayMax[firstTicket].r, arrayMax[firstTicket].c, &arrayMax[firstTicket]);
    ticket++;

    scanf("%d", &r2); //Scans the second matrix
    scanf("%d", &c2);
    matrixInitializer(r2, c2, ticket);
    matrixBuilder(arrayMax[ticket].r, arrayMax[ticket].c, &arrayMax[ticket]);
    matrixInitializer(arrayMax[firstTicket].r, arrayMax[ticket].c);
    matrixMultiplier(arrayMax[firstTicket])

    return 0;
}
