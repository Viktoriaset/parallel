#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
	int rank, n, i, message;
	double startTime, endTime;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// в этой лабе засекали время выполнения предыдущей 
	startTime = MPI_Wtime();
	for (i = 0; i < n; i++)
	{
		if (rank == 0 && i == 0)
		{
			MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			cout << "Send message to 1 " << endl;

			MPI_Recv(&message, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Process 0 get message:  \"" << message << "\" from " << 1 << endl;
		}
		else if (rank == 1 && i == 1)
		{
			MPI_Recv(&message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Process 1 get message: \"" << message << "\" from " << 0 << endl;

            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			cout << "Send message to 0 " << endl;
		}
	}
	endTime = MPI_Wtime();

	MPI_Finalize();

	cout << "process " << rank << " finished for time = " << (endTime - startTime)/n << endl;

	return 0;
}