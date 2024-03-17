#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../reseau/server.c"
#include "lib/header.h"
#include <limits.h>


const int DEPTH_MAX = 6;

char lettre1[20]="";
int fin=0;
int tour=1;

int tableau [SIZE][SIZE];


int main() {
    affichage(tableau,tour);
    tableau[6][6]=init(tableau);

    int clientsocket=mainserv();

    char choixjoueur[20];
    printf("Choose your option (Player/AI) : ");
    scanf("%s",choixjoueur);

    // Main loop for the game
    while (fin!=5)
    {
  
    
    printf("Turn = %d \n",tour%2!=0);
    // Test to know it it's a even or odd turn
    if(tour%2==0){
        // For an even turn (so it's the client's turn) :
        // If it's a player then we ask him for his move and print it if it's a possible move
        if(strcmp(choixjoueur, "Player")==0){
            // Asking for the player's move
            printf("Which case do you want to color ? ");
            scanf(" %2s",&lettre1[0]); // The empty space before '%2s' is important as it allows the program not to loop (consumme la prochaine ligne)
            printf("MOVE TO PLAY : %s !!!!,%lu\n",&lettre1[0],strlen(lettre1));
            

            struct ResultatValable resultat = valable(lettre1,tableau);
            
            // Tests for the validity of the move and if is then we edit the board game and send the data
            if(resultat.valide){
                tableau[6][6]=editTableau(tableau,resultat.colonne,resultat.ligne,tour);
                tour++;

                // Sends the data to the client
                char messageaenvoyer[3];
                sprintf(messageaenvoyer, "%d%d",resultat.colonne,resultat.ligne);
                printf("We send : message = %s column = %d line = %d \n",messageaenvoyer,resultat.colonne,resultat.ligne);
                
                send_data_to_client(clientsocket,messageaenvoyer);
            }else {
                // If the move is not valid then we warn the player and the loop restart until a move is possible
                printf("Move not valid...\n");
                
            }
        // If it's an AI then we it's making his move and sending it to the client
        }else{
           // The AI makes its move by using the alpha-beta algorithm
            SearchResult result = alphabeta(tableau, DEPTH_MAX, 2, INT_MIN, INT_MAX, 1);
            editTableau (tableau, result.move.colonne, result.move.ligne, tour);
            tour++;

            // Prints the move made by the AI
            printf("The AI played the move : (%d, %d).\n", result.move.ligne, result.move.colonne);

            // Sends the message to the client
            char messageaenvoyer[3];
            sprintf(messageaenvoyer, "%d%d",result.move.colonne,result.move.ligne);
            printf("We send : message = %s column = %d line = %d \n",messageaenvoyer,result.move.colonne,result.move.ligne);

            send_data_to_client(clientsocket,messageaenvoyer);
        }
    }else{
        // For an odd turn (so it's the client's turn) :
        
        // Receives the move of the client
        char messagerecu[BUFFER_SIZE]="";
        printf("MESSAGERECU %s %lu \n",messagerecu,strlen(messagerecu));

        // Loop waiting for a message (not NULL) to arrive (refresh every 2 seconds)
        while (strlen(messagerecu)==0){
            handle_client((int)(clientsocket),messagerecu);
            sleep(2);
        }
        

        printf("Réception de %s\n",(messagerecu));

        // Updates the board game with the data received from the client
        struct ResultatValable resultat;
        resultat.colonne=messagerecu[0];
        resultat.ligne=messagerecu[1];
        printf("Move at : %d %d\n",resultat.colonne,resultat.ligne);
        tableau[6][6]=editTableau(tableau,resultat.colonne-'0',resultat.ligne-'0',tour);
        tour++;
    }  

    // Prints the game board
    affichage(tableau,tour);

    // Checks if there is a winner, loops again if there isn't 
    if (verifyVictory(tableau, (tour%2)+1) == 1)
    {
        printf ("\n\t\tThe player %d WON !!!\n", tour%2 + 1);
    }
    else
    {
        printf ("\n\t\tThe player %d didn't won... for now...\n", tour%2 + 1);
    }
    }
}
