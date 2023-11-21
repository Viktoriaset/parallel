#include <iostream>
#include <omp.h>

int arrA[1000000], arrB[1000000];

int main()
{
    int a[10];
    int maxElement = -1;

    for (int i = 0; i < 10; i++)
    {
        a[i] = i;
    }

#pragma omp parallel reduction(max:maxElement)
    {
        int localMax = 0;

    #pragma omp for 
        for (int i = 0; i < 10; i++)
        {
            if (localMax < a[i])
            {
                localMax = a[i];
            }
        }

    #pragma omp critical 
        {
            if (localMax > maxElement)
            {
                maxElement = localMax;
            }
        }
    }

    printf("Max = %d \n", maxElement);

    printf("_________________________ RunTime section _________________________ \n");

#pragma omp parallel num_threads(10)
    {
    #pragma omp for
        for (int i = 0; i < 1000000; i++)
        {
            arrA[i] = rand();
        }
    }

    for (int i = 1; i <= 10; i++)
    {
        double startTime, endTime;
        maxElement = INT_MIN;
        startTime = omp_get_wtime();
    #pragma omp parallel num_threads(i)
        {
            int localMax = INT_MIN;

        #pragma omp for
            for (int i = 0; i < 1000000; i++)
            {
                localMax = arrA[i] < localMax ? localMax : arrA[i];
            }

        #pragma omp critical 
            {
                maxElement = maxElement < localMax ? localMax : maxElement;
            } 
        }
        endTime = omp_get_wtime();

        printf("QThreads = %d, time = %lf, maxElement = %d \n", i, endTime - startTime, maxElement);
    }
}
