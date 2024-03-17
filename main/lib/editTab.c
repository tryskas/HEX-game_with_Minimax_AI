#include "header.h"
#include  <string.h>

int editTableau(int tableau[][SIZE],int indexlettre1,int indexlettre2,int tour) {
    int val =0;
    if (tour%2==0){
        val=2;
    }else{
        val=1;
    }
    tableau[indexlettre2][indexlettre1]=val;
    return tableau[SIZE][SIZE];
}

struct ResultatValable valable(const char prompt[], int tableau[][SIZE]) {
    struct ResultatValable resultat;
    resultat.valide = 0;

    char *alphabettableau = "ABCDEF";
    char *alphabetpetittableau = "abcdef";
    char *nombretableau = "123456";

    char lettre1bis = (char)(prompt[0]);
    char lettre2bis = (char)(prompt[1]);

    char *trouve1 = strchr(alphabettableau, lettre1bis);
    char *trouve1bis = strchr(alphabetpetittableau, lettre1bis);
    char *trouve2 = strchr(nombretableau, lettre2bis);

    int chiffre1 = (int)(lettre1bis);
    int chiffre2 = (int)(lettre2bis);

    int taille = strlen(prompt);


    if ((trouve1 != NULL||trouve1bis != NULL) && (taille == 2)) {
        if (trouve2 != NULL) {
            //printf("test %d %d %d %d\n", *trouve1, *trouve2, chiffre1, chiffre2);
            
            if (trouve1bis != NULL) { // cas lettre minuscule
                int indexLettre1min = lettre1bis - 'a';
                int indexLettre2min = *trouve2 -'1';
                int casetableau = tableau[indexLettre2min][indexLettre1min];
                if (casetableau == 0) {
                    // la case est vide donc on peut faire des trucs
                    resultat.valide = 1;
                    resultat.colonne = indexLettre1min;
                    resultat.ligne = indexLettre2min;
                }
            } else { // cas lettre majuscule
                int indexLettre1maj = lettre1bis - 'A';
                int indexLettre2maj = *trouve2 -'1';
                int casetableau = tableau[indexLettre2maj][indexLettre1maj];
                if (casetableau == 0) {
                    // la case est vide donc on peut faire des trucs
                    resultat.valide = 1;
                    resultat.colonne = indexLettre1maj;
                    resultat.ligne = indexLettre2maj;
                }
            }
        }
    }
    //printf("resultat = %d %d %d \n",resultat.valide,resultat.lettre1,resultat.lettre2);
    return resultat;
}