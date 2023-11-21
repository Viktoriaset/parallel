#include "mpi.h"
#include <iostream>

using namespace std;

void printArray(auto* array, int len)
{
    for (int i = 0; i < len; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    int size, rank, position, i;
    float a[10];
    char b[10], buf[100];
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    for (i = 0; i < 10; i++)
    {
        a[i] = rank + 1.0;
        if (rank == 0)
            b[i] = 'a';
        else
            b[i] = 'b';
    }
    
    position = 0;
    
    if (rank == 0)
    {
        MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(b, 10, MPI_CHAR, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
    }
    else
    {
        cout << "_________________\n" << "rank = " << rank << "\n";

        cout << "mass a" << endl;
        printArray(a, 10);

        cout << "mass b" << endl;
        printArray(b, 10);
        
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
        position = 0;
        MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buf, 100, &position, b, 10, MPI_CHAR, MPI_COMM_WORLD);

        cout << "unpacked" << endl;

        cout << "mass a" << endl;
        printArray(a, 10);

        cout << "mass b" << endl;
        printArray(b, 10);

        cout << "_________________\n";
    }


    
    MPI_Finalize();
}