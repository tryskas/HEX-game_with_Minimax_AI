#include "header.h"
#include <stdio.h>

void affichage(int tableau[SIZE][SIZE],int tour){
    
    int i,j=0;
    printf("NOUS SOMMES AU %d IEME TOUR\n--------------------\n",tour);
    for(i=0; i<SIZE; ++i){
        for(j=0; j<SIZE; ++j){
            printf("%d ",tableau[i][j]);
        }
        printf("\n");
    }
}