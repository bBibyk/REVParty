/**
 * @file main.c
 * @brief Analyse de la ligne de commande et appel de la fonction appropriée, gestion des sorties
 * @author Bibyk Bogdan
 * @date 31 octobre 2023
 *
 * Fonctions de gestion de la ligne de commande et du noyau fonctionnel.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "lecture_csv.h"
#include "condorcet.h"
#include "jugement_majoritaire.h"
#include "uninominales.h"

////////////////////////////////////////////////////////
// -- Fonctions de gestion de la ligne de commande -- //
////////////////////////////////////////////////////////

//To be removed

//End to be removed

char *getParameters(int argc, char *argv[]){
}

void checkParameters();

int main(int argc, char *argv[])
{
    bool duel = false;
    char inputFile[MAXCHAR];
    char logFile[MAXCHAR];
    char method[MAXCHAR];

    int option;
    while((option = getopt(argc, argv, "i:d:o:m:")) != -1){
        switch(option){
            case 'i':
                duel = false;
                strcpy(inputFile, optarg);
            case 'd':
                duel = true;
                strcpy(inputFile, optarg);
            case 'o':
                strcpy(logFile, optarg);
            case 'm':
                strcpy(method, optarg);
            case '?':
                fprintf(stderr, "Usage: -i|-d nom_fichier [-o nom_fichier] -m méthode\n");
                // exit(EXIT_FAILURE);
        }
    }

    printf("Input is:\n%s > %i\n%s\n%s\n", inputFile, duel, logFile, method);
}