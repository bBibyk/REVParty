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
#include "utils.c"

////////////////////////////////
// -- Fonctions auxilières -- //
////////////////////////////////

/**
 * @fn static FILE *openFile(char *path)
 * @brief Fonction d'ouverture d'un fichier en mode lecture seule
 * @param[in] path Chemin du fichier à ouvrir
 * @return Pointeur vers le fichier ouvert
 *
 * @note Cette fonction affiche un message d'erreur si le fichier n'a pas pu être ouvert.
 */
FILE *openFileWrite(char *path){
    FILE *fp = fopen(path, "w");
    if (fp == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/**fopen
 * @fn bool isIn(char *string, char *tab[], int lenTab)
 * @brief Fonction de recherche d'une valeur dans un tableau de chaînes de caractères.
 * @param[in] string Valeur à rechercher.
 * @param[in] tab Tableau de chaînes de caractères dans lequel effectuer la recherche.
 * @param[in] lenTab Taille du tableau.
 * @return true si la valeur est trouvée, false sinon.
**/
bool stringInArray(char *string, char *tab[], int lenTab){
    for(int i=0; i<lenTab; i++){
        if(strcmp(string, tab[i]) == 0){
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////
// -- Fonctions de gestion de la ligne de commande -- //
////////////////////////////////////////////////////////

/**
 * @fn void getParameters(int argc, char *argv[], bool *duel, char *inputFile, char *logFile, char *method)
 * @brief Fonction de récupération des options et des arguments.
 * @param[in] argc Nombre d'arguments donnés.
 * @param[in] atgv Tableau d'arguments donnés.
 * @param[out] duel Indicateur de duel.
 * @param[out] inputFile Chemin du fichier d'entrée.
 * @param[out] logFile Chemin du fichier de log.
 *
 * @note Cette fonction affiche un message d'erreur si une option n'est pas valide.
 */
void getParameters(int argc, char *argv[], bool *duel, char *inputFile, char *logFile, char *method){
    int option;
    while((option = getopt(argc, argv, "i:d:o:m:")) != -1){
        switch(option){
            case 'i':
                *duel = false;
                strcpy(inputFile, optarg);
                break;
            case 'd':
                *duel = true;
                strcpy(inputFile, optarg);
                break;
            case 'o':
                strcpy(logFile, optarg);
                break;
            case 'm':
                strcpy(method, optarg);
                break;
            case '?':
                fprintf(stderr, "Usage: -i|-d nom_fichier -m méthode [-o nom_fichier]\n");
                exit(EXIT_FAILURE);
        }
    }
}

/**
 * @fn bool checkParameters(bool duel, char inputFile, char logFile, char method)
 * @brief Fonction de verification de validité des paramètres.
 * @param[in] duel Indicateur de duel.
 * @param[in] methods Tableau des méthodes.
 * @param[in] lenMethods Taille du tableau.
 * @param[in] inputFile Chemin du fichier d'entrée.
 * @param[in] logFile Chemin du fichier de log.
 * @param[in] method Nom de la méthode.
 * 
 * @note Cette fonction affiche un message d'erreur si une option est manquante ou si un argument n'est pas valide.
**/
void checkParameters(bool duel, char *methods[], int lenMethods, char *inputFile, char *method){
    if(inputFile==NULL || method==NULL){
        fprintf(stderr, "Usage: -i|-d et -m sont des méthodes obligatoires\n");
        exit(EXIT_FAILURE);
    }
    if(!stringInArray(method, methods, lenMethods)){
        fprintf(stderr, "Usage: méthode doit être dans [");
        for(int i=0; i<lenMethods; i++){
            fprintf(stderr, "%s", methods[i]);
            if(i<lenMethods-1){
                fprintf(stderr, ", ");
            }
        }
        fprintf(stderr, "]\n");
        exit(EXIT_FAILURE);
    }
}

////////////////
// -- MAIN -- //
////////////////

int main(int argc, char *argv[]){
    bool duel = false;
    char inputFile[MAXCHAR];
    char logFile[MAXCHAR];
    char method[MAXCHAR];
    bool debugMode = false;

    getParameters(argc, argv, &duel, inputFile, logFile, method);
    if(duel){
        char *methods[] = {"cm", "cp", "cs", "jm", "all"};
        int lenMethods = 5;
        checkParameters(duel, methods, 5, inputFile, method);
    }else{
        char *methods[] = {"uni1", "uni2", "cm", "cp", "cs", "jm", "all"};
        int lenMethods = 7;
        checkParameters(duel, methods, 7, inputFile, method);
    }

    if(logFile){
        debugMode = true;
        FILE *log = openFileWrite(logFile);
    }

    DataFrame *df = createDataFrameFromCsv(inputFile);

    if(strcmp(method, "uni1") == 0){
        printResult(voteUninominalUnTour(NULL, NULL, NULL), method, 1);
    }else if(strcmp(method, "uni2") == 0){
        printf("uni2");
    }else if(strcmp(method, "cm") == 0){
        printf("Not implemented yet\n");
    }else if(strcmp(method, "cp") == 0){
        printf("Not implemented yet\n");
    }else if(strcmp(method, "cs") == 0){
        printf("Not implemented yet\n");
    }else if(strcmp(method, "jm") == 0){
        printf("Not implemented yet\n");
    }else{
        printf("and all other methods...\n");
    }
}