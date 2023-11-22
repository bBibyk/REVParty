/**
 * @file utils.h
 * @brief Fonctions utiles pour les différents systèmes de votes.
 * @author Essengue Matis
 * @date 11 novembre 2023
 *
 * Fichier d'en-tête pour les fonctions utiles pour les différents systèmes de votes.
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
    int score;
    char winner[MAXCHAR];
} VoteResult;

#endif // UTILS_H