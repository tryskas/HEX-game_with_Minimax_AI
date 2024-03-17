#ifndef GAME_H
#define GAME_H

#define SIZE 6

typedef struct
{
    int ligne;
    int colonne;
}Move;

typedef struct
{
    int value;
    Move move;
} SearchResult;

// Structure pour stocker le résultat de valable
struct ResultatValable {
    int valide;
    int colonne;
    int ligne;
};

int init();
struct ResultatValable valable(const char prompt[], int tableau[][SIZE]);
void affichage(int tableau[SIZE][SIZE], int tour);
int editTableau(int tableau[][SIZE], int indexlettre1, int indexlettre2, int tour);
int isValidPosition(int x, int y);
int verifyVictory(int tab[][SIZE], int player);
int evaluate(int board[][SIZE], int player, int isServeur);
SearchResult alphabeta(int board[][SIZE], int depth, int joueur, int alpha, int beta, int isServeur);
int countMobility(int board[][SIZE], int player);

#endif