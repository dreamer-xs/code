#include <stdio.h>


int main(void)
{
    int i;
    int j;
    
    for (i =1 ; i < 10; i++)
    {
        for (j =1 ; j <= i; j++)
        {
            printf("%2d * %2d = %2d  ",j, i ,i*j);
        }
        printf("\n");
    }



    return 0;
}
