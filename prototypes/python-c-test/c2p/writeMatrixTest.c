#include <stdio.h>


int main()
{
     FILE *fp = fopen("tmp.csv", "w");

     double myArray[3][3];

     for(int i = 0; i < 3; i++)
     {
          for(int j = 0; j < 3; j++)
          {
               myArray[i][j] = (double) 3*i+1+j;
          }
     }

     for(int i =0; i< 3; i++)
     {
          for(int j=0; j<3; j++)
          {
               fprintf(fp,"%f",myArray[i][j]);
               if(j!=3-1)
                    fprintf(fp,",");
          }
          fprintf(fp,"\n");
     }

     fclose(fp);

}
