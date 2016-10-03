#include <stdio.h>
#include <stdlib.h>

int main(void) {

     system("python3 testCtypes.py");

     FILE *fp;

     fp = fopen("testFile.dat", "rb");

     int arraySize;

     fread(&arraySize,sizeof(int),1,fp);

     double myArray[arraySize][arraySize];

     fread(&myArray,sizeof(myArray),1,fp);
     fclose(fp);

     for(int i = 0; i < arraySize; i++)
     {
          for(int j = 0; j < arraySize; j++)
          {
               printf(" %f ",myArray[i][j]);
          }
          printf("\n");
     }

}
