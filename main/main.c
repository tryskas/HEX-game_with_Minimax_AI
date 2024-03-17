#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
int main() {
    char choix[20];
 
    printf("Choisissez l'option (client/serveur) : ");
    scanf("%s", choix);
 
    if (strcmp(choix, "client") == 0) {
        printf("Lancement de mainclient.c...\n");
        system("gcc mainclient.c lib/*.c -o mainclient.exe && ./mainclient.exe"); // Exécuter mainclient
    } else if (strcmp(choix, "serveur") == 0) {
        printf("Lancement de mainserv.c...\n");
        system("gcc mainserv.c lib/*.c -o mainserv.exe && ./mainserv.exe"); // Exécuter mainserv
    } else {
        printf("Choix invalide.\n");
        return 1;
    }
 
    return 0;
}