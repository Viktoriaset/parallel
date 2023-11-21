#include <iostream>
#include "C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h"

using namespace std;

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	// get quntity programm
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	// get rank this programm
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank == 0)
	{
		cout << "Hello from process " << rank << "\n";
		for (i = 1; i < n; i++) {
			// waiting datat from any_source with any tag
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Hello from process " << message << endl;
		}
	}  // send data on 0 source with 0 tag 
	else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}