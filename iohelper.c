#include "lif.h"


void exportArrayToCSV(char* filename, double* myArray, int length)
{
     printf("Begin Export\n");
     FILE *fp = fopen(filename,"w");

     for(int i = 0; i < length; i++)
     {
          fprintf(fp, "%f", myArray[i]);
          if(i != length-1)
               fprintf(fp,",");
     }
     fclose(fp);
     printf("End Export\n");
}

void export2DArrayToCSV(char* filename, double** myArray, int width, int height)
{
     FILE *fp = fopen(filename,"w");

     for(int i = 0; i < height; i++)
     {
          for(int j = 0; j < width; j++)
          {
               fprintf(fp, "%f", myArray[i][j]);
               if(j != width - 1)
                    fprintf(fp,",");
          }
          fprintf(fp,"\n");
     }
     fclose(fp);
}
