#include <stdio.h>

int main(void) {
     FILE *fp;

     double myArray[3][3];

     fp = fopen("testFile.dat", "rb");

     fread(&myArray,sizeof(myArray),1,fp);
     fclose(fp);

     for(int i = 0; i < 3; i++)
     {
          for(int j = 0; j < 3; j++)
          {
               printf("%f",myArray[i][j]);
          }
     }

}
