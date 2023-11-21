#include <iostream>
#include <omp.h>

int arrA[1000000], arrB[1000000];

int main()
{
    int a[10], b[10];

#pragma omp parallel
    {
    #pragma omp for 
        for (int i = 0; i < 10; i++)
        {
            a[i] = 2;
            b[i] = 2;
        }
    }

    int sum = 0;
#pragma omp parallel 
    {
        int threadNumber = omp_get_thread_num();

    #pragma omp for
        for (int i = 0; i < 10; i++)
        {
            int product = a[i] * b[i];

        #pragma omp atomic
            sum += product;
            printf("Thread: %d products elements with number: %d and get %d \n", threadNumber, i, product);
        }

    
    }

    printf("sum = %d \n", sum);
    

    printf("____________________________________ RunTime test ____________________________________ \n");


#pragma omp parallel 
    {
    #pragma omp for
        for (int i = 0; i < 1000000; i++)
        {
            arrA[i] = rand();
            arrB[i] = rand();
        }
    }

    for (int i = 1; i <= 10; i++)
    {
        double startTime, endTime;
        long long scalarSum = 0;
        startTime = omp_get_wtime();
        
    #pragma omp parallel num_threads(i)
        {
            
        #pragma omp for
            for (int i = 0; i < 1000000; i++)
            {
                int product = arrA[i] * arrB[i];

            #pragma omp atomic
                scalarSum += product;
            }
            
        }
        endTime = omp_get_wtime();

        printf("QTHreads = %d, time = %lf, scalarSum = %llu \n", i, endTime - startTime, scalarSum);
    }
}