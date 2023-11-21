#include <cstdlib>
#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

void printMatrix(int** matrix, int len)
{
    for (int i =0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int** createMatrix(int len)
{
    int** result = new int*[len];
    for (int i = 0; i < len; i++)
    {
        result[i] = new int[len];
    }

    for (int i =0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            result[i][j] = 1;
        }
    }

    return result;
}

int** matrixMultiple(int** matrix1, int** matrix2, int len)
{
    int** result = createMatrix(len);

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            int sum = 0;
            for (int k = 0; k < len; k++)
            {
                sum += matrix1[i][k] * matrix2[k][j];
            }

            result[i][j] = sum;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    int rank, size;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int** matrix1 = createMatrix(10);
    int** matrix2 = createMatrix(10);

    MPI_Barrier(MPI_COMM_WORLD); // барьерная синхронизация процессов
    // начало замера времени для каждого процесса
    start_time = MPI_Wtime();
    // перемножение матриц
    matrixMultiple(matrix1, matrix2, 10);
    // конец замера времени
    end_time = MPI_Wtime();

    cout << end_time - start_time << endl;
    // вывод время выполнения перемножения матриц на каждом процессе
    MPI_Finalize();
}