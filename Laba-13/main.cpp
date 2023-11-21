#include <iostream>
#include <mpi.h>

using namespace std;

int mass1[1000000];
int mass2[1000000];

void fillMassIdentity(int *mas, int len)
{
    for (int i = 0; i < len; i++)
    {
        mas[i] = 1;
    }
}

int *getMassForProcess(int *mas, int processRank, int masLen)
{
    int *result = (int *)malloc(masLen * sizeof(int));

    for (int i = 0; i < masLen; i++)
    {
        result[i] = mas[i + masLen * (processRank - 1)];
    }

    return result;
}

int vectorScalarMultiple(int *arr1, int *arr2, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += arr1[i] * arr2[i];
    }

    return sum;
}

void CommandForZeroProcess(int massLen, int quntityProcess, MPI_Status status, int &sum)
{
    // делим массив на части для их отправки другим программам
    int qElementsOnProcess = massLen / (quntityProcess - 1);
    for (int i = 1; i < quntityProcess; i++)
    {
        int *sendingMass1 = getMassForProcess(mass1, i, qElementsOnProcess);
        int *sendingMass2 = getMassForProcess(mass2, i, qElementsOnProcess);

        // отправляем по 2 массива на каждую программу 
        MPI_Send(sendingMass1, qElementsOnProcess, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(sendingMass2, qElementsOnProcess, MPI_INT, i, 1, MPI_COMM_WORLD);
    }

    // досчитываем остальные элементы массива 
    int qElementsOnZeroProcess = massLen % (quntityProcess - 1);
    for (int i = 0; i < qElementsOnZeroProcess; i++)
    {
        sum += mass1[i + ((quntityProcess - 1) * qElementsOnProcess)] * mass2[i + ((quntityProcess - 1) * qElementsOnProcess)];
    }

    for (int i = 1; i < quntityProcess; i++)
    {
        int localSum;
        // скалярное произвдение той части массива что отправили по программам
        MPI_Recv(&localSum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sum += localSum;
    }
}

void CommandForProcessWithoutZero(int massLen, int quntityProcess, MPI_Status status)
{
    int qElementsOnProcess = massLen / (quntityProcess - 1);

    int *arr1 = (int *)malloc(qElementsOnProcess * sizeof(int));
    int *arr2 = (int *)malloc(qElementsOnProcess * sizeof(int));

    // получаем массивы 
    MPI_Recv(arr1, qElementsOnProcess, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(arr2, qElementsOnProcess, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

    int result = vectorScalarMultiple(arr1, arr2, qElementsOnProcess);

    // отправляем скалярное произведение массивов
    MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

int main(int argc, char **argv)
{
    int rank, quntityProcess, ibuf, massLen, sum;
    MPI_Status status;
    float rbuf;

    massLen = 1000000;
    fillMassIdentity(mass1, massLen);
    fillMassIdentity(mass2, massLen);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &quntityProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sum = 0;

    if (rank == 0)
    {
        CommandForZeroProcess(massLen, quntityProcess, status, sum);
    }
    else
    {
        CommandForProcessWithoutZero(massLen, quntityProcess, status);
    }

    MPI_Finalize();

    if (rank == 0)
    {
        cout << "Scalar multiple = " << sum << endl;
    }
}