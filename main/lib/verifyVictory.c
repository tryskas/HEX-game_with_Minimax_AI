#include "header.h"

int isValidPosition (int x, int y)
{
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

//verifie la condition de victoire
int sideConnected(int tab[SIZE][SIZE], int x, int y, int player, int visited [6][6])
{ 
    if (!isValidPosition(x, y))
    {
        return 0;
    }

    visited[x][y] = 1;

    if ((player == 1 && y == 5) || (player == 2 && x == 5))
    {
        return 1;
    }

    for (int yCoo = y-1; yCoo <= y+1; yCoo++)
    {
        for (int xCoo = x-1; xCoo <= x+1; xCoo++)
        {
            if (xCoo != x && yCoo != y)
            {
                if (tab[xCoo][yCoo] == player && visited[xCoo][yCoo] == 0)
                {
                    if (sideConnected (tab, xCoo, yCoo, player, visited) == 1)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

//utilise le principe de recherhce en profondeur
int verifyVictory (int tab[][SIZE], int player)
{
    int count = 0;
    //verification egalité
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (tab[i][j] == 0)
            {
                count ++;
            }
        }
    }

    if (count == 36)
    {
        return 2;
    }

    int visited [SIZE][SIZE] = {{0}};

    if (player == 2)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if ((tab [0][j] == player) && (sideConnected(tab, 0, j, player, visited) == 1))
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        for (int j = 0; j < SIZE; j++)
        {
            if ((tab [j][0] == player) && (sideConnected(tab, j, 0, player, visited) == 1))
            {
                return 1;
            }
        }
        return 0;
    }
}