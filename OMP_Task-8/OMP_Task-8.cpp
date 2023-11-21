#include <iostream>
#include <omp.h>

int arr[10][10], arrBig[1000][1000];
int result[10][10];
long long resultBig[1000][1000];

int main()
{
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            arrBig[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            arr[i][j] = 1;
        }
    }

#pragma omp parallel 
    {
    #pragma omp for
        for (int i = 0; i < 10; i++)
        {
        #pragma omp parallel
            {
            #pragma omp for
                for (int j = 0; j < 10; j++)
                {
                    int arrIJ = 0;
                    for (int k = 0; k < 10; k++)
                    {
                        arrIJ += arr[i][k] * arr[k][j];
                    }

                #pragma omp critical 
                    {
                        result[i][j] = arrIJ;
                    }
                }
            }
        
        }
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    double timeOnOneThread = 0;

    for (int QThreads = 1; QThreads <= 10; QThreads++)
    {
        double startTime, endTime;

        startTime = omp_get_wtime();
    #pragma omp parallel num_threads(QThreads)
        {
        #pragma omp for collapse(2)
            for (int i = 0; i < 1000; i++)
            {
                for (int j = 0; j < 1000; j++)
                {
                    long long arrIJ = 0;

                    for (int k = 0; k < 1000; k++)
                    {
                        arrIJ += arrBig[i][k] * arrBig[k][j];
                    }

                #pragma omp critical 
                    {
                        resultBig[i][j] = arrIJ;
                    }
                }
            }
        }
        endTime = omp_get_wtime();

        if (QThreads == 1)
        {
            timeOnOneThread = endTime - startTime;
        }

        printf("QThreads = %d, time = %lf, effectiveness = %lf \n", QThreads, endTime - startTime, timeOnOneThread / (endTime - startTime));
    }

}


