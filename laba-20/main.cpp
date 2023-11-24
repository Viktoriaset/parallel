
#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, i, rbuf;
    int ranks[128], new_rank;

    MPI_Init(&argc, &argv);

    MPI_Group group, new_group;
    MPI_Comm new_comm;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_group(MPI_COMM_WORLD, &group);// получаем текущую группу 

    for (i = 0; i < size / 2; i++)
        ranks[i] = i;

    if (rank < size / 2)
        MPI_Group_incl(group, size / 2, ranks, &new_group); // Создаем новыую группу из процессов с рангом из переменой ranks
    else
        MPI_Group_excl(group, size / 2, ranks, &new_group);// исключаем из старой группы процессы с рангом из переменой ranks

    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm); // создаем новый коммуниккатор
    MPI_Allreduce(&rank, &rbuf, 1, MPI_INT, MPI_SUM, new_comm); // отправляем всем слушателям коммуникатора новые данные
    MPI_Group_rank(new_group, &new_rank);

    cout <<  "rank= " << rank << ", newrank= " << new_rank << ", rbuf= " << rbuf << endl;
    
    MPI_Finalize();
}