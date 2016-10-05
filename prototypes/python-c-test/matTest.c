#include<stdio.h>
#include<stdlib.h>

int main(){

int i, j;

double **mat;

  mat = malloc(2 *sizeof(double *));

for(i=0;i<2;i++){

  mat[i] = malloc(3 *sizeof(double));

        for(j=0;j<3;j++){

        printf("Input a value for Array[%d][%d]: ",i,j);

        scanf("%lf",&mat[i][j]);

        }
  }

for(i=0;i<2;i++)
  for(j=0;j<3;j++)
  printf("%f\t",mat[i][j]);

return 0;
}
