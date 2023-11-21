
#include <iostream>
#include <omp.h>

int main()
{
    int sum = 0;
    int a[10];

    for (int i = 0; i < 10; i++)
    {
        a[i] = 1;
    }

#pragma omp parallel reduction (+: sum)
    {
        int localSum = 0;
    #pragma omp for 
        for (int i = 0; i < 10; i++)
        {
            sum += a[i];
        }
    }

    printf("Quntity threads: %d \n", sum);

    sum = 0;
#pragma omp parallel 
    {
        int localSum = 0;
    #pragma omp for
        for (int i = 0; i < 10; i++)
        {
            localSum += a[i];
        }

    #pragma omp atomic 
        sum += localSum;
    }

    printf("sum = %d \n", sum);

    sum = 0;
#pragma omp parallel 
    {
        int localSum = 0;
    #pragma omp for
        for (int i = 0; i < 10; i++)
        {
            localSum += a[i];
        }

    #pragma omp critical 
        {
            sum += localSum;
        }
    }

    printf("critical sum = %d \n", sum);
}