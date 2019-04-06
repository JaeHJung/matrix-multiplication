#include <stdio.h>

int main(int argc, char *argv[]){
    int row, col, i, j, k, init, counter;
    counter = 1;
    scanf("%d", &row);
    scanf("%d", &col);
    scanf("%d", &init);
    //Matrix builder
    for(k = 0; k < init; k++){
        counter = 1;
        printf("%d %d\n", row, col);
        for (i = 0; i < row; i++){
            for(j = 0; j < col; j++){
                printf("%d ", counter);
            }
            printf("\n");
            counter++;
        }
    }
    printf("0 0");
}