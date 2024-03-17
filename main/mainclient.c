#include "lib/header.h"
#include "../reseau/client.c"
#include <limits.h>


#define BUFFER_SIZE 1024

const int DEPTH_MAX = 6;

char lettre1[20]="";
int fin=0;
int tour=1;

int tableau [SIZE][SIZE];

void main() {
    affichage(tableau,tour);
    tableau[6][6] = init(tableau);
    int clientsocket = mainclient();



    char choixjoueur[20];
    printf("Choississez l'option(Joueur/IA) : ");
    scanf("%s",choixjoueur);

    

    //Boucle de jeu 
    while (fin!=1)
    {
    //Prompt
    printf("tour = %d \n",tour%2!=0);
    if(tour%2!=0){
        //SI c'est un joueur
        if(strcmp(choixjoueur, "Joueur") == 0){
            //Tour impair client
            printf("Quelle case veux-tu colorer ?");
            scanf(" %2s",&lettre1[0]);//l'espace a son importance permet de pas boucler (consumme la prochaine ligne)
            printf("COUP A JOUER: %s !!!!\n",&lettre1[0]);
            struct ResultatValable resultat;
            resultat = valable(lettre1,tableau);
            
            //const char* message="A1";
            if(resultat.valide){
                //printf("Coup valable en %d %d\n",resultat.colonne,resultat.ligne);
                tableau[6][6] = editTableau(tableau,resultat.colonne,resultat.ligne,tour);
                tour++;

                //envoie à l'autre la donnée
                char messageaenvoyer[3];
                sprintf(messageaenvoyer, "%d%d",resultat.colonne,resultat.ligne);
                printf("on envoie message=%s lettre1=%d lettre2=%d \n",messageaenvoyer,resultat.colonne,resultat.ligne);
                
                send_data_to_server(clientsocket,messageaenvoyer);
                //sendserv(connfdserv,messageaenvoyer);
            }else {
                printf("pas valide\n");
                //envoie à l'autre que c'est pas valide
                
                //printf("Coup pas valable en %d %d\n",resultat.colonne,resultat.ligne);
            }
        }else{

             // L'IA effectue son coup en utilisant alphabeta
            SearchResult result = alphabeta(tableau, DEPTH_MAX, 1, INT_MIN, INT_MAX, 2);
            editTableau (tableau, result.move.colonne, result.move.ligne, tour);           
            tour++;

            // Afficher l'emplacement du meilleur coup de l'IA
            printf("L'IA a joué en (%d, %d).\n", result.move.ligne, result.move.colonne);

            char messageaenvoyer[3];
            sprintf(messageaenvoyer, "%d%d",result.move.colonne,result.move.ligne);
            printf("on envoie message=%s lettre1=%d lettre2=%d \n",messageaenvoyer,result.move.colonne,result.move.ligne);

            send_data_to_server(clientsocket,messageaenvoyer);
        }
    }else{
        //tour pair serveur
        
        //reception donnée de l'autre
        char messagerecu[BUFFER_SIZE]="";
        printf("MESSAGERECU %s %lu \n",messagerecu,strlen(messagerecu));
        int fin2=0;
        while (strlen(messagerecu)==0 && fin2!=5){
            //fin2++;
            //printf("wait for awsner...");
            handle_server((int)(clientsocket),messagerecu);
            sleep(2);
            //printf("reception de %s\n",(messagerecu));
        }
        printf("reception de %s\n",(messagerecu));
        struct ResultatValable resultat;
        resultat.colonne=messagerecu[0];
        resultat.ligne=messagerecu[1];
        printf("Coup %d %d\n",resultat.colonne,resultat.ligne);  
        tableau[6][6]=editTableau(tableau,resultat.colonne-'0',resultat.ligne-'0',tour);
        tour++;            
        

    //Affichage du tableau 
    
    affichage(tableau,tour);
    //debut condition de victoire
    if (verifyVictory(tableau, (tour%2)+1) == 1)
    {
        printf ("\n\t\tle joueur %d a GAGNER !!!\n", tour%2 + 1);
    }
    else
    {
        printf ("\n\t\tle joueur %d n'a pas encore gagner  \n", tour%2 + 1);
    }
    }
    }
}



