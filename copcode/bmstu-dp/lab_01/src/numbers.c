#include "numbers.h"

// get nth Fibonacci number
int fib32(int n)
{
    if (n > MAX_N)
    {
        puts("48 is the maximum ordinal for int32.");
        return OVERFLOW;
    }
    else if (n < MIN_N)
    {
        puts("1 is the minimum ordinal.");
        return INCORRECT;
    }

    int num1 = FIRST, num2 = SECOND, sum;

    for (int i = 0; i < n - 1; i++)
    {
        sum = num1 + num2;
        num1 = num2;
        num2 = sum;
    }

    return num1;
}
