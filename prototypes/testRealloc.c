#include <stdio.h>
#include <stdlib.h>

int main()
{
     int * a;

     a = calloc(20, sizeof(int));

     for(int i = 0; i < 20; i++)
     {
          a[i] = i;
     }

     // printf("Size a: %i\n", sizeof(*a));
     for(int i = 0; i < 20; i++)
     {
          printf("%i ",a[i]);
     }
     printf("\n");


     a = realloc(a, 10 * sizeof(int));

     // printf("Size b: %i\n", sizeof(*b));
     for(int i = 0; i < 10; i++)
     {
          printf("%i ", a[i]);
     }
     printf("\n");


     return 0;
}
