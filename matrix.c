#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

//int trap = 0; // Boolean variable to check if the input is 0 0; Default to false.
uint64_t diffr = 0;
uint64_t diffw = 0;
uint64_t diffc = 0;
uint64_t diffall = 0;
struct timespec startRead, endRead, startCompute, endCompute, startWrite, endWrite, startall, endall;

typedef struct mat_t {
   int r;
   int c;
   uint64_t *m;
} mat_t;

mat_t arrayMax[100];

// Building an empty matrix
int matrixbuilder(int nrow, int ncol, uint64_t Nmatrix[nrow][ncol]){
    clock_gettime(CLOCK_REALTIME, &startRead);
    int i, j;
    for(i = 0; i < nrow; i++){
        for(j =0; j < ncol; j++){
            scanf("%ld", &Nmatrix[i][j]); 
            /* printf("%ld\n", Nmatrix[i][j]); */ 
        }
    }
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    return 0;
}

// Printing out the matrix
int printMatrix(mat_t *m){
    int i, j;
    printf("PRINTING THE DANG MATRIX\n");
    /* for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            printf("%ld ", m[i);
            if (j == c2 - 1){
                printf("\n");
            }
        }
    } */
    for (i=0; i<m->r; i++) {
        for (j=0; j<m->c; j++){
            printf("%ld ",m->m[i*m->c+j]);
        }
        printf("\n");
    }
    printf("END OF THE PRINT\n");
    return 0;
}

// Building memeory space for the matrix
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

// Sanitizing the tmpMatrix for reuse
int matrixRebuilder(int nrow, int ncol, uint64_t Nmatrix[nrow][ncol], uint64_t tmpMatrix[nrow][ncol]){
    clock_gettime(CLOCK_REALTIME, &startCompute);
    int i, j;
    //Resetting the tmpMatrix to 0
    for (i = 0; i < nrow; i++){
        for (j = 0; j < ncol; j++){
            tmpMatrix[i][j] = 0;
        }
    }

    for (i = 0; i < nrow; i++){
        for ( j = 0; j < ncol; j++){
            /* printf("Nmatrix: %ld\n", Nmatrix[i][j]); */
            tmpMatrix[i][j] += Nmatrix[i][j];
            /* printf("tmpMatrix: %ld\n", tmpMatrix[i][j]); */
        }
    }

    clock_gettime(CLOCK_REALTIME, &endCompute);
    diffc += 1000000000 * (endCompute.tv_sec - startCompute.tv_sec) + endCompute.tv_nsec - startCompute.tv_nsec;
    return 0;
}

// Matrix multiplier
int matrixmultiplier(int r1, int c1, int r2, int c2, uint64_t mtrx1[r1][c1], uint64_t mtrx2[r2][c2], uint64_t tmpMatrix[r1][c2]){
    clock_gettime(CLOCK_REALTIME, &startCompute);
    int i, j, k;

    // Initialzing the temp matrix
    for (i = 0; i < r1; i++){
        for (j = 0; j < c2; j++){
            tmpMatrix[i][j] = 0;
        }
    }
    
    printf("Multiplying\n");
    for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            for(k = 0; k < c1; k++){
                tmpMatrix[i][j] += mtrx1[i][k] * mtrx2[k][j];
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &endCompute);
    diffc += 1000000000 * (endCompute.tv_sec - startCompute.tv_sec) + endCompute.tv_nsec - startCompute.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &startWrite);

    for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            printf("%ld ", tmpMatrix[i][j]);
            if (j == c2 - 1){
                printf("\n");
            }
        }
    }

    clock_gettime(CLOCK_REALTIME, &endWrite);
    diffw += 1000000000 * (endWrite.tv_sec - startWrite.tv_sec) + endWrite.tv_nsec - startWrite.tv_nsec;
    //printf("%ld\n", diffw);
    return 0;
}

// Main Function
int main(int argc, char *argv[]){
    
    int r1, c1, r2, c2, rchain, cchain;
    int counter;
    int ticket = 0;
    int rTemp;
    int cTemp[256];
    
    //Scanning and computing the first two matrixes
    clock_gettime(CLOCK_REALTIME, &startall);
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &r1); //Scans the first matrix

    scanf("%d", &c1);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

    uint64_t mtrx1[r1][c1];
    //matrixInitializer(r1, c1, ticket);
    //printMatrix(&arrayMax[ticket]); // Printing the Matix
    matrixbuilder(r1, c1, mtrx1);
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &r2); //Scans the second matrix
    scanf("%d", &c2);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

    uint64_t mtrx2[r2][c2];
    uint64_t tmpMtrx[r1][c2];

    matrixbuilder(r2, c2, mtrx2);
    /* printf("r1: %d c1: %d r2: %d c2: %d\n", r1,c1,r2,c2); */
    /* printf("First Matrix Multiplier\n"); */
    matrixmultiplier(r1, c1, r2, c2, mtrx1, mtrx2, tmpMtrx);
  /*   printf("///////////////////////\n"); */

    //Begins the chaining
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &rchain); //Scans the next available matrix
    scanf("%d", &cchain);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

    //Debugging for 0 0
    /* if (rchain == 0 && cchain == 0){
        printf("Chain test: %d %d\n", rchain, cchain);
    } */

    counter = 0;
    rTemp = r1;
    cTemp[counter] = c2;

    while(rchain != 0 && cchain != 0){ //Chekcs to see if its termination by '0 0'
        uint64_t mtrxChain[rchain][cchain];
        matrixbuilder(rchain, cchain, mtrxChain);
        uint64_t blankMatrix[rTemp][cchain];
        matrixmultiplier(rTemp, cTemp[counter], rchain, cchain, tmpMtrx, mtrxChain, blankMatrix);

        counter++;
        cTemp[counter] = cchain;
        matrixRebuilder(rTemp, cchain, blankMatrix, tmpMtrx);
        clock_gettime(CLOCK_REALTIME, &startRead);
        scanf("%d", &rchain); //Scans the next available matrix
        scanf("%d", &cchain);

        //Debugging
        //printf("R = %d C = %d\n", rchain, cchain);

        clock_gettime(CLOCK_REALTIME, &endRead);
        diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    }
    clock_gettime(CLOCK_REALTIME, &endall);
    diffall += 1000000000 * (endall.tv_sec - startall.tv_sec) + endall.tv_nsec - startall.tv_nsec;
    uint64_t total = diffr + diffc + diffw;
    uint64_t difference = diffall - total;
    printf("Reading    = %lu nanoseconds\n", diffr);
    printf("Computing  = %lu nanoseconds\n", diffc);
    printf("Writing    = %lu nanoseconds\n", diffw);
    printf("Total add  = %lu nanoseconds\n", total);
    printf("Total time = %lu nanoseconds\n", diffall);
    printf("Difference = %lu nanoseconds\n", difference);


    return 0;
}