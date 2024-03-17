#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int dx[] = {-1, 1, 1, -1};
static int dy[] = {-1, -1, 1, 1};


int countMobility(int board[][SIZE], int player)
{
    int mobility = 0;
    
    // Parcourez la grille pour compter le nombre de mouvements possibles
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                // Case vide, ce joueur peut y jouer
                mobility++;
            }
        }
    }
    
    return mobility;
}


int calculateAverageDistanceToEdges(int board[][SIZE], int player) {
    int totalDistance = 0;
    int numPieces = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == player) {
                int distance = MIN(i, SIZE - 1 - i) + MIN(j, SIZE - 1 - j);
                totalDistance += distance;
                numPieces++;
            }
        }
    }

    // Évitez une division par zéro
    if (numPieces == 0) {
        return 0;
    }

    // Calculez la distance moyenne aux bords
    int avgDistance = totalDistance / numPieces;
    return avgDistance;
}

// évaluation 
int evaluate (int board[][SIZE], int player, int isServeur)
{
    if (verifyVictory (board, player) == 1)
    {
        if (isServeur)
        {
            if (player == 2)
            {
                return INT_MAX;
            }
            else
            {
                return INT_MIN;
            }   
        }
        else
        {
            if (player == 1)
            {
                return INT_MAX;
            }
            else
            {
                return INT_MIN;
            }   
        }
    }

    int playerScore = 0;
    int oponentScore = 0;

    //controle du centre
    int centerX = SIZE/2;
    int centerY = SIZE/2;

    if (isServeur)
    {  
        if (board[centerX][centerY] == 2)
        {
            playerScore += 20;
        }
        else if (board[centerX][centerY] == 1)
        {
            oponentScore += 20;
        }
    }
    else
    {
            if (board[centerX][centerY] == 1)
        {
            playerScore += 20;
        }
        else if (board[centerX][centerY] == 2)
        {
            oponentScore += 20;
        }
    }

    // Recherche de motifs de pièces alignées en diagonale
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == player) {


                for (int direction = 0; direction < 4; direction++) {
                    int count = 1; // Nombre de pièces alignées
                    int x = i + dx[direction];
                    int y = j + dy[direction];

                    while (isValidPosition(x, y) && board[x][y] == player) {
                        count++;
                        x += dx[direction];
                        y += dy[direction];
                    }

                    // Attribution de scores en fonction du nombre de pièces alignées
                    if (count >= 3) {

                        if (isServeur)
                        {
                            if (player == 2) {

                                playerScore += 100; // Exemple : joueur 2 a un motif de 3 pièces alignées en diagonale
                            } else {
                                oponentScore += 100; // Exemple : joueur 1 a un motif de 3 pièces alignées en diagonale
                            }
                        }
                        else
                        {
                            if (player == 1) {

                                playerScore += 100; // Exemple : joueur 2 a un motif de 3 pièces alignées en diagonale
                            } else {
                                oponentScore += 100; // Exemple : joueur 1 a un motif de 3 pièces alignées en diagonale
                            }
                        }
                    }

                    if (count == 2) {
                        if (isServeur)
                        {
                            if (player == 2) 
                            {
                                playerScore += 50; // Joueur 2 peut potentiellement bloquer Joueur 1
                            } else 
                            {
                                oponentScore += 50; // Joueur 1 peut potentiellement bloquer Joueur 2
                            }
                        }
                        else
                        {
                            if (player == 1) 
                            {
                                playerScore += 50; // Joueur 1 peut potentiellement bloquer Joueur 2
                            } else 
                            {
                                oponentScore += 50; // Joueur 2 peut potentiellement bloquer Joueur 1
                            }
                        }
                    }
                }
            }
        }
    }


// Évaluation de la mobilité des pièces du joueur
    int mobility = countMobility(board, player);
    
    // Ajout de points en fonction de la mobilité
    if (isServeur)
    {
        playerScore += mobility * 10; // Vous pouvez ajuster le facteur de pondération
    }
    else
    {
        oponentScore += mobility * 10; // Vous pouvez ajuster le facteur de pondération
    }


    // Détection de la possibilité de bloquer l'adversaire
    if (isServeur)
    {
        int opponent = (player == 1) ? 2 : 1;

        // Parcourez le plateau pour vérifier si l'adversaire peut gagner au prochain tour
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (board[i][j] == 0)
                {
                    // Case vide, essayez de simuler le mouvement de l'adversaire
                    board[i][j] = opponent;

                    // Vérifiez si l'adversaire peut gagner après ce mouvement
                    if (verifyVictory(board, opponent) == 1)
                    {
                        // L'adversaire peut gagner au prochain tour si nous ne bloquons pas
                        // Vous pouvez attribuer un malus ici, par exemple :
                        oponentScore -= 100;
                    }

                    // Annulez le mouvement simulé
                    board[i][j] = 0;
                }
            }
        }
    }

    int proximityScore = calculateAverageDistanceToEdges(board, player);
    if (isServeur)
    {
        if (player == 2)
        {
            playerScore += proximityScore;
        }
        else
        {
            oponentScore += proximityScore;
        }
    }
    else
    {
        if (player == 1)
        {
            playerScore += proximityScore;
        }
        else
        {
            oponentScore += proximityScore;
        }
    }

    return playerScore - oponentScore;
}


SearchResult alphabeta(int board[][SIZE], int depth, int joueur, int alpha, int beta, int isServeur) {
    
    SearchResult result;
    
    //si on est le serveur, l'IA sera le joueur 2
    //sinon on est le client et l'IA sera le joueur 1
    if (isServeur)
    {
        result.value = (joueur == 2) ? INT_MIN : INT_MAX;
    }

    else
    {
        result.value = (joueur == 1) ? INT_MIN : INT_MAX;
    }

    Move bestMove;
    bestMove.ligne = -1;
    bestMove.colonne = -1;

    // Cas de base : si la profondeur est nulle ou il y a une victoire
    if (depth == 0 || verifyVictory(board, joueur) == 1) {
        result.value = evaluate(board, joueur, isServeur);
        return result;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                // Effectuez le mouvement hypothétique
                board[i][j] = joueur;

                // Appel récursif pour évaluer le mouvement
                SearchResult moveResult = alphabeta(board, depth - 1, (joueur == 1) ? 2 : 1, alpha, beta, isServeur);

                // Annulez le mouvement hypothétique
                board[i][j] = 0;

                // Mettez à jour le meilleur mouvement si nécessaire
                if (isServeur)
                {
                    if (joueur == 2) 
                    {
                        if (moveResult.value > result.value) 
                        {
                            result.value = moveResult.value;
                            bestMove.ligne = i;
                            bestMove.colonne = j;
                        }
                        alpha = (alpha > result.value) ? alpha : result.value;
                        if (beta <= alpha) 
                        {
                            result.move = bestMove;
                            return result; // Élagage Beta
                        }
                    } 
                    else 
                    {
                        if (moveResult.value < result.value) 
                        {
                            result.value = moveResult.value;
                            bestMove.ligne = i;
                            bestMove.colonne = j;
                        }
                        beta = (beta < result.value) ? beta : result.value;
                        if (beta <= alpha) 
                        {
                            result.move = bestMove;
                            return result; // Élagage Alpha
                        }
                    }
                }
                else
                {
                    if (joueur == 1) 
                    {
                        if (moveResult.value > result.value) 
                        {
                            result.value = moveResult.value;
                            bestMove.ligne = i;
                            bestMove.colonne = j;
                        }
                        alpha = (alpha > result.value) ? alpha : result.value;
                        if (beta <= alpha) 
                        {
                            result.move = bestMove;
                            return result; // Élagage Beta
                        }
                    } 
                    else 
                    {
                        if (moveResult.value < result.value) 
                        {
                            result.value = moveResult.value;
                            bestMove.ligne = i;
                            bestMove.colonne = j;
                        }
                        beta = (beta < result.value) ? beta : result.value;
                        if (beta <= alpha) 
                        {
                            result.move = bestMove;
                            return result; // Élagage Alpha
                        }
                    }
                }
            }
        }
    }

    // Si la meilleure position est toujours la valeur par défaut (-1, -1), choisissez une position valide par défaut
    if (bestMove.ligne == -1 && bestMove.colonne == -1) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    bestMove.ligne = i;
                    bestMove.colonne = j;
                    break;
                }
            }
            if (bestMove.ligne != -1 && bestMove.colonne != -1) {
                break;
            }
        }
    }

    result.move = bestMove;
    return result;
}

