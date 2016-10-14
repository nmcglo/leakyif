#include <stdio.h>
#include <stdlib.h>

int main()
{
     double aDouble = .003 * 1000;
     double anIntJitter = ((int) aDouble) + .0003;

     printf("%f",anIntJitter);



     return 0;
}
