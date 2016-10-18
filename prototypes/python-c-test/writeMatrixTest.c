#include <stdio.h>


int main()
{
     FILE *fp = fopen("tmp.dat", "wb");

     double myArray[3][3];

     for(int i = 0; i < 3; i++)
     {
          for(int j = 0; j < 3; j++)
          {
               myArray[i][j] = i+j*3;
          }
     }

     fwrite(myArray, sizeof(double),sizeof(myArray),fp);
     fclose(fp);


}
