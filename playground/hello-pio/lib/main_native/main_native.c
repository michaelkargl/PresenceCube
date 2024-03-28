#include "stdio.h"
#include "math_util.h"

int main()
{
    int a = 123;
    int b = 321;
    int sum = add(a, b);
    
    printf("------------------------------\n");
    printf("Sum of %i + %i = %i\n", a, b, sum);
    printf("------------------------------\n");

    return 0;
}