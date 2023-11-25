#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <cstdlib>

using namespace std;

char** createEmptyGameField()
{
    char** gameField = (char**)malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++)
    {
        gameField[i] = (char*)malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++)
        {
            gameField[i][j] = ' ';
        }
    }

    return gameField;
}

void drawGameField(char ** gameField)
{
    system("CLS");

    for (int i = 0; i < 3; i++)
    {
        cout << "_______" << endl;
        for (int j = 0; j < 3; j++)
        {
            cout << "|" << gameField[i][j];
        }
        cout << "|" << endl;
    }

    cout << "_______" << endl;
}

void InputXAndY(int &x, int &y, char** gameField)
{
    cout << "Please enter x and y your moove:";
    cin >> x >> y;
    if (x < 1 || x > 3 || y < 1 || y > 3)
    {
        drawGameField(gameField);
        cout << "ERROR: incorect x or y please input 1 <= x <= 3 and 1 <= y <= 3";
        InputXAndY(x, y, gameField);
    }
}

bool CheckWinInDirection(int x, int y, int directionX, int directionY, char** gameField)
{
    char serchingChar = gameField[x][y];
    int charInCombination = 1;
    
    x += directionX; y += directionY;

    while(x >= 0 && x < 3 && y >= 0 && y < 3)
    {
        if (gameField[x][y] == serchingChar)
        {
            charInCombination++;
            x += directionX; y += directionY;
        }
        else 
        {
            return charInCombination < 3 ? false : true;
        }
    }

    return charInCombination < 3 ? false : true;
}

int CheckWin(int x, int y, char** gameField)
{
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (CheckWinInDirection(x, y, i, j, gameField))
            {
                return 1;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameField[i][j] == ' ')
            {
                return 0;
            }
        }
    }

    return -1;
}

void SetXByCoordinate(int x, int y, char** gameField)
{
    gameField[x, y] = "X";

}



int main(int argc, char **argv)
{
    int r, rank, send_messsage = 25;
    int x, y;
    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm intercomm;

    char** gameField = createEmptyGameField();

    MPI_Open_port(MPI_INFO_NULL, port_name);

    MPI_Publish_name("example", MPI_INFO_NULL, port_name);

    cout << "Ожидание клиента..." << endl;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    cout << "Клент поключился" << endl;

    bool isServerFirst = rand() % 2 == 0 ? true : false;

    MPI_Send(&isServerFirst, 1, MPI_C_BOOL, 0, 0, intercomm);

    if (isServerFirst) 
    {
        InputXAndY(x,  y, gameField);
        gameField[x][y] = 'X';
    } else 
    {
        MPI_Recv(&x, 1, MPI_INT, 0, 0, intercomm, &status);
        MPI_Recv(&y, 1, MPI_INT, 0, 1, intercomm, &status);
        gameField[x][y] = 'O';
    }

    int gameState = 0;

    while (gameState == 0)
    {
        InputXAndY(x,  y, gameField);
        gameField[x][y] = 'X';
        MPI_Send(&x, 1, MPI_INT, 0, 0, intercomm);
        MPI_Send(&y, 1, MPI_INT, 0, 1, intercomm);

        gameState = CheckWin(x, y, gameField);

        if (gameState == 1)
        {
            cout << "You win!" << endl;
        } else if (gameState == -1)
        {
            cout << "Nobody win" << endl;
        }

        MPI_Recv(&x, 1, MPI_INT, 0, 0, intercomm, &status);
        MPI_Recv(&y, 1, MPI_INT, 0, 1, intercomm, &status);
        gameField[x][y] = 'O';

        drawGameField(gameField);

        gameState = CheckWin(x,  y, gameField);

        if (gameState == 1)
        {
            cout << "You loose!" << endl;
        } else if (gameState == -1)
        {
            cout << "Nobody win" << endl;
        }

    }

    MPI_Unpublish_name("example", MPI_INFO_NULL, port_name);
    MPI_Close_port(port_name);
    MPI_Comm_free(&intercomm);

    MPI_Finalize();
    return 0;
}