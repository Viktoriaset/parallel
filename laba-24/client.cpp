#include <stdio.h>
#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int rank, message_from_server, message_for_server = 42;

    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;
    MPI_Status status;

    MPI_Lookup_name("example", MPI_INFO_NULL, port_name);

    cout << "Попытка подключения" << endl;
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    cout << "Соединение с сервером" << endl;

    MPI_Comm_rank(intercomm, &rank);
    cout << "rank = " << rank << endl;

    MPI_Send(&rank, 1, MPI_INT, 0, 1, intercomm);
    cout << "Клент отправил значение " << message_for_server << endl;

    MPI_Recv(&message_from_server, 1, MPI_INT, 0, 0, intercomm, &status);
    cout << "Клиент получил занчение " << message_from_server << endl;

    MPI_Finalize();

    return 0;
}