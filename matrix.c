#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
//int trap = 0; // Boolean variable to check if the input is 0 0; Default to false.
uint64_t diffr = 0;
uint64_t diffw = 0;
uint64_t diffc = 0;
struct timespec startRead, endRead, startCompute, endCompute, startWrite, endWrite;

int matrixbuilder(int nrow, int ncol, int Nmatrix[nrow][ncol]){
    int i, j;
    clock_gettime(CLOCK_REALTIME, &startRead);
    for(i = 0; i < nrow; i++){
        for(j =0; j < ncol; j++){
            scanf("%d", &Nmatrix[i][j]); 
        }
    }
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    return 0;
}

int matrixmultiplier(int r1, int c1, int r2, int c2, int mtrx1[r1][c1], int mtrx2[r2][c2], int tmpMatrix[r1][c2]){
    int i, j, k;
    clock_gettime(CLOCK_REALTIME, &startCompute);
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
    clock_gettime(CLOCK_REALTIME, &endCompute);
    diffc += 1000000000 * (endCompute.tv_sec - startCompute.tv_sec) + endCompute.tv_nsec - startCompute.tv_nsec;

    clock_gettime(CLOCK_REALTIME, &startWrite);
    for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            printf("%d ", tmpMatrix[i][j]);
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


int main(int argc, char *argv[]){
    int r1, c1, r2, c2, rchain, cchain, rTemp, cTemp;

    //Scanning and computing the first two matrixes
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &r1); //Scans the first matrix
    scanf("%d", &c1);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

    int mtrx1[r1][c1];

    matrixbuilder(r1, c1, mtrx1);
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &r2); //Scans the second matrix
    scanf("%d", &c2);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

    int mtrx2[r2][c2];
    int tmpMtrx[r1][c2];

    matrixbuilder(r2, c2, mtrx2);
    matrixmultiplier(r1, c1, r2, c2, mtrx1, mtrx2, tmpMtrx);

    //Begins the chaining
    clock_gettime(CLOCK_REALTIME, &startRead);
    scanf("%d", &rchain); //Scans the next available matrix
    scanf("%d", &cchain);
    clock_gettime(CLOCK_REALTIME, &endRead);
    diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;
    printf("rchain : %d || cchain : %d\n", rchain, cchain);
    if (rchain == 0 && cchain == 0){
        printf("Chain test: %d %d\n", rchain, cchain);
    }
    rTemp = r1;
    cTemp = c2;
    int counter = 0;
    while(rchain != 0 && cchain != 0){ //Chekcs to see if its termination by '0 0'
        counter++;
        printf("loop %d\n", counter);
        int mtrxChain[rchain][cchain];
        matrixbuilder(rchain, cchain, mtrxChain);
        int blankMatrix[rTemp][cchain];
        matrixmultiplier(rTemp, cTemp, rchain, cchain, tmpMtrx, mtrxChain, blankMatrix);
        cTemp = cchain;

        clock_gettime(CLOCK_REALTIME, &startRead);
        scanf("%d", &rchain); //Scans the next available matrix
        scanf("%d", &cchain);
        clock_gettime(CLOCK_REALTIME, &endRead);
        diffr += 1000000000 * (endRead.tv_sec - startRead.tv_sec) + endRead.tv_nsec - startRead.tv_nsec;

        printf("rchain : %d || cchain : %d\n", rchain, cchain);
    }
    printf("Reading = %lu nanoseconds\n", diffr);
    printf("Computing = %lu nanoseconds\n", diffc);
    printf("Writing = %lu nanoseconds\n", diffw);
    return 0;
}