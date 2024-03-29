#include "stdio.h"
#include "add_util/add_util.h"
#include "subtract_util/subtract_util.h"

int main()
{
    int left = 125;
    int right = 326;
    printf("------------------------------\n");
    printf("%i + %i = %i\n", left, right, add_util__add(left, right));
    printf("%i - %i = %i\n", left, right, subtract_util__subtract(left, right));
    printf("------------------------------\n");

    return 0;
}