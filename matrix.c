#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int trap = 0; // Boolean variable to check if the input is 0 0; Default to false.

int matrixbuilder(int nrow, int ncol, int Nmatrix[nrow][ncol]){
    int i, j;
    for(i = 0; i < nrow; i++){
        for(j =0; j < ncol; j++){
            scanf("%d", &Nmatrix[i][j]);
            printf("%d\n", Nmatrix[i][j]);  
        }
        if (Nmatrix[i][j] == 0){
            printf("%d\n", Nmatrix[i][j]);
            trap = 1;
            printf("Reached 0 0\n");
        } else {
            trap = 0;
        }
    }
    return 0;
}

int matrixmultiplier(int r1, int c1, int r2, int c2, int mtrx1[r1][c1], int mtrx2[r2][c2], int tmpMatrix[r1][c2]){
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
    for (i = 0; i < r1; i++){
        for(j = 0; j < c2; j++){
            printf("%d", tmpMatrix[i][j]);
            if (j == c2 - 1){
                printf("\n");
            }
        }
    }
    return 0;
}

int main(){
    int r1, c1, r2, c2;

    scanf("%d", &r1);
    scanf("%d", &c1);

    int mtrx1[r1][c1];

    matrixbuilder(r1, c1, mtrx1);
    scanf("%d", &r2);
    scanf("%d", &c2);

    int mtrx2[r2][c2];
    int tmpMtrx[r1][c2];

    matrixbuilder(r2, c2, mtrx2);
    matrixmultiplier(r1, c1, r2, c2, mtrx1, mtrx2, tmpMtrx);
    return 0;
}