
#include "header.h"
#include <stdio.h>

#ifndef INIT
#define INIT

int init(int tableau [SIZE][SIZE]){
//Initialisation du tableau
    int i,j=0;
    for(i=0; i<SIZE; ++i){
        for(j=0; j<SIZE; ++j){
            tableau[i][j]=0;
            //printf("%d",tableau[i][j]);
        }
    }
    return tableau[SIZE][SIZE];
}

#endif