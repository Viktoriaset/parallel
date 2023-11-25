#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <fstream>

using namespace std;

#define BUFSIZE 100

char random_char()
{
    return 'a' + rand() % 26;
}

char* random_string(int len)
{
    char* string = (char*) malloc(len * sizeof(char));
    for (int i = 0; i < len; i++)
    {
        string[i] = random_char();
    }

    return string;
}

void create_random_file(char* name)
{
    std::ofstream outfile (name);
    for (int i = 0;i < 1 + rand()%12; i++)
    {
        outfile << random_string(1 + rand() % 10) << endl;
    }
    outfile.close();
}

void MPI_File_close_and_delete(MPI_File f, char* filename)
{
    MPI_File_close(&f);
    remove(filename);
}

int main(int argc, char **argv)
{
    int bufsize, num, sum;

    MPI_Init(&argc, &argv);

    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];
    char* filename = "file.txt";

    create_random_file(filename);

    MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

    sum = 0;
    do
    {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        printf("buf=%s\n", buf);
        sum += num;
    } while (num >= BUFSIZE);

   MPI_File_close_and_delete(fh, filename);

    cout << sum << endl;

    MPI_Finalize();
}