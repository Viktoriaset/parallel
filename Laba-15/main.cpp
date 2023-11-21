#include <iostream>
#include <mpi.h>
#include <math.h>

using namespace std;

void fillMassIdentity(int *mas, int len)
{
    for (int i = 0; i < len; i++)
    {
        mas[i] = 1;
    }
}

int *sumVector(int *leftArr, int *rightArr, int len)
{
    int *result = (int *)malloc(len * sizeof(int));
    for (int i = 0; i < len; i++)
    {
        result[i] = leftArr[i] + rightArr[i];
    }

    return result;
}

int* CascadeSum(int qProcess, int rank, int deep, int *arr, int arrLen)
{
    MPI_Status status;
    int qRecvProcess = qProcess / pow(2, deep);
    if ((rank > 0 && rank < qRecvProcess) || rank == 0)
    {
        if (qRecvProcess == 0)
        {
            return arr;
        }

        int *newArr = (int *)malloc(arrLen * sizeof(int));
        MPI_Recv(newArr, arrLen, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int* result = sumVector(newArr, arr, arrLen);

        return CascadeSum(qProcess, rank, deep + 1, result, arrLen);
    }
    else
    {
        MPI_Send(arr, arrLen, MPI_INT, rank - qRecvProcess, 0, MPI_COMM_WORLD); // 0 1 2 3 4 5 6 7 // 0 1 2 3 // 0 1
        return arr;
    }

    return arr;
}

int main(int argc, char **argv)
{
    int rank, quntityProcess, massLen;

    massLen = 1000000;

    int *mass = (int *)malloc(massLen * sizeof(int));

    fillMassIdentity(mass, massLen);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &quntityProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* result = CascadeSum(quntityProcess, rank, 1, mass, massLen);

    if (rank == 0)
    {
        for (int i = 0; i < 5; i++)
        {
            cout << result[i] << endl;
        }
    }

    MPI_Finalize();

    return 0;
}