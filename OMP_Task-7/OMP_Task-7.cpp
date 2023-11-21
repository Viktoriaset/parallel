#include <iostream>
#include <omp.h>
#include <math.h>

int main()
{
    double pi = 0;
    double startTime, endTime;
    long long n;

    std::cin >> n;

    startTime = omp_get_wtime();

#pragma omp parallel num_threads(1)
    {
        double localPi = 0;

    #pragma omp for 
        for (int i = 0; i < n; i++)
        {
            localPi += 4 / (1 + pow((i + 0.5) / n, 2));
        }

    #pragma omp atomic
        pi += localPi;
    }

    pi = pi / n;

    endTime = omp_get_wtime();
    printf("Qthreads = 1, time = %lf, pi = %lf \n", endTime - startTime, pi);

    pi = 0;
    startTime = omp_get_wtime();

#pragma omp parallel num_threads(8)
    {      
        double localPi = 0;

    #pragma omp for 
        for (int i = 0; i < n; i++)
        {
            localPi += 4 / (1 + pow((i + 0.5) / n, 2));
        }

    #pragma omp atomic
        pi += localPi;
    }

    pi = pi / n;

    endTime = omp_get_wtime();
    printf("Qthreads = 8, time = %lf, pi = %lf \n", endTime - startTime, pi);
}
