#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, prev, next;
    int buf[2];

    MPI_Init(&argc, &argv);

    MPI_Request reqs[4];
    MPI_Status stats[4];

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    prev = rank - 1;
    next = rank + 1;

    if (rank == 0)
        prev = size - 1;
    if (rank == size - 1)
        next = 0;

    MPI_Sendrecv(&rank, 1, MPI_INT, next, 6, &buf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &stats[0]);
    MPI_Sendrecv(&rank, 1, MPI_INT, prev, 5, &buf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &stats[1]);
    
    cout << "Message from prev =" << buf[0] << " Message from next" << buf[1] << endl;

    MPI_Finalize();
}