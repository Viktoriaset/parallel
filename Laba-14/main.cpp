#include <iostream>
#include <mpi.h>
#include <math.h>

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

int *getMassForProcess(int *mas, int masLen)
{
    int *result = (int *)malloc(masLen * sizeof(int));

    for (int i = 0; i < masLen; i++)
    {
        result[i] = mas[i];
    }

    return result;
}

void CommandForZeroProcess(int massLen, MPI_Status status)
{
    int *sendingMass1 = getMassForProcess(mass1, massLen);

    for (int i = 0; i < 10; i++)
    {
        MPI_Send(sendingMass1, massLen, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(sendingMass1, massLen, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    }
}

void CommandForFirstProcess(int massLen, MPI_Status status)
{
    int *arr1 = (int *)malloc(massLen * sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        MPI_Recv(arr1, massLen, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Send(arr1, massLen, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
    int rank, quntityProcess, massLen;
    MPI_Status status;
    double start_time, end_time;

    massLen = 1000000;
    fillMassIdentity(mass1, massLen);
    fillMassIdentity(mass2, massLen);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &quntityProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 1; i <= 6; i++)
    {
        int len = pow(10, i);
        // измеряем скорость передачи данных на массиво разной длинны, запускать с количеством программ равным 2
        start_time = MPI_Wtime();

        if (rank == 0)
        {
            CommandForZeroProcess(len, status);
        }
        else
        {
            CommandForFirstProcess(len, status);
        }

        end_time = MPI_Wtime();

        if (rank == 0)
        {
            cout << "N = 10, L = " << len * sizeof(int)
                 << " T = " << (end_time - start_time)
                 << " R = " << 2 * 10 * len * sizeof(int) / (end_time - start_time) << endl;

            cout << "final i = " << i << endl;
        }
    }

    MPI_Finalize();

    return 0;
}