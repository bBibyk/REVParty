/**
 * @file utils.h
 * @brief Fonctions et structures utiles pour les différents systèmes de votes.
 * @authors Essengue Matis
 * @date 11 novembre 2023
 *
 * Fichier avec les utiles pour les différents systèmes de votes.
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "lecture_csv.h"

/////////////////////////////////
// -- Structures de données -- //
/////////////////////////////////

typedef struct VoteResult
{
    int nb_candidates;
    int nb_voters;
    float score;
    char winner[MAXCHAR];
} VoteResult;

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
void printResult(VoteResult result, char *method, int tour);

#endif // UTILS_H
