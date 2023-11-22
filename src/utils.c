/**
 * @file utils.c
 * @brief Fonctions et structures utiles pour les différents systèmes de votes.
 * @authors Bibyk Bogdan
 * @date 11 novembre 2023
 *
 * Fichier avec les utiles pour les différents systèmes de votes.
 *
 */

#ifndef UTILS_C
#define UTILS_C

#include "utils.h"

/////////////////////
// -- Fonctions -- //
/////////////////////

/**
 * @fn void printResult(VoteResult result, char *method, int tour)
 * @brief Fonction pour afficher les résultats d'un vote.
 * @param[in] result Structure contenant les données du vote.
 * @param[in] method Mode de scrutin.
 * @param[in] tour Tour du vote, si uninominal.
 * 
 * @note L'affichage est base sur le mode de scrutin.
 */
void printResult(VoteResult result, char *method, int tour){
    printf("Mode de scrutin : ");

    if(strcmp(method, "uni1") == 0){
        printf("uninominal à un tour, tour %d", tour);
    }else if(strcmp(method, "uni2") == 0){
        printf("uninominal à deux tours, tour %d", tour);
    }else if(strcmp(method, "cm") == 0){
        printf("Condorcet minimax");
    }else if(strcmp(method, "cp") == 0){
        printf("Condorcet paires");
    }else if(strcmp(method, "cs") == 0){
        printf("Condorcet Schulze");
    }else if(strcmp(method, "jm") == 0){
        printf("Jugement majoritaire");
    }

    printf(", %d candidats, %d votants, vainqueur = %s", result.nb_candidates, result.nb_voters, result.winner);

    if(strcmp(method, "uni1") == 0 || strcmp(method, "uni2") == 0){
        printf(", score = %d", result.score);
    }

    printf("\n");
}

#endif // UTILS_C
