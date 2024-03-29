#include "multiplication_util.h"
#include "add_util/add_util.h"

// so proud, definately gonna put this into my next CV
int minmax_util__multiply(int number, int multiplicant)
{
    int product = 0;
    for (int i = 1; i <= multiplicant; i++)
    {
        product = add_util__add(product, number);
    }
    return product;
}