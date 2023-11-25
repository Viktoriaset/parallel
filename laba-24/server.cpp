#include <stdio.h>
#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    int r, rank, send_messsage = 25;
    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm intercomm;

    MPI_Open_port(MPI_INFO_NULL, port_name);

    MPI_Publish_name("example", MPI_INFO_NULL, port_name);

    cout << "Ожидание клиента..." << endl;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    cout << "Клент поключился" << endl;

    MPI_Comm_rank(intercomm, &rank);
    cout << "rank = " << rank << endl;

    MPI_Send(&send_messsage, 1, MPI_INT, 0, 0, intercomm);
    cout << "Сервер отправил значение " << send_messsage << endl;

    MPI_Recv(&r, 1, MPI_INT, 0, 1, intercomm, &status);
    cout << "Сервер получил сообщение " << r << endl;

    MPI_Unpublish_name("example", MPI_INFO_NULL, port_name);
    MPI_Close_port(port_name);
    MPI_Comm_free(&intercomm);

    MPI_Finalize();
    return 0;
}