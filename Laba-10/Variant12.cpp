#include <iostream>
#include <mpi.h>

using namespace std;

void getMessageAndSendNext(int rankSendler, int rankAdressee)
{
	MPI_Status status;
	int message, rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Recv(&message, 1, MPI_INT, rankSendler, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	cout << "Get message " << message << " from process " << rankSendler << endl;
	MPI_Send(&message, 1, MPI_INT, rankAdressee, 0, MPI_COMM_WORLD);
	cout << "Process " << rank << " send message next " << endl;
}

int main(int argc, char *argv[])
{

	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (i = 0; i < n; i++)
	{
		if (rank == 0 && i == 0)
		{
			MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			cout << "Send message next " << endl;

			MPI_Recv(&message, 1, MPI_INT, n - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Process 1 get messag " << message << " from " << n - 1 << endl;
		}
		else if (rank == n - 1 && i == n - 1)
		{
			getMessageAndSendNext(rank - 1, 0);
		}
		else if (i == rank)
		{
			getMessageAndSendNext(rank - 1, rank + 1);
		}
	}

	MPI_Finalize();

	return 0;
}