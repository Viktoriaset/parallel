#include <iostream>
#include <mpi.h>

using namespace std;

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
			// send on programm 1 
			MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			cout << "Send message to 1 " << endl;

			// recerv from programm 1
			MPI_Recv(&message, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Process 0 get message:  \"" << message << "\" from " << 1 << endl;
		}
		else if (rank == 1 && i == 1)
		{
			// recev from programm 0
			MPI_Recv(&message, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Process 1 get message: \"" << message << "\" from " << 0 << endl;

			// send on programm 0
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			cout << "Send message to 0 " << endl;
		}
	}

	MPI_Finalize();

	return 0;
}