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
#include <string.h>
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

/**
 * @fn void logprintf(FILE *log, bool debugMode, char *message, ...)
 * @brief Fonction pour afficher les messages de debug.
 * @param[in] log Fichier de log.
 * @param[in] debugMode Mode de debug.
 * @param[in] message Message à afficher.
 * @param[in] ... Arguments du message.
 */
void logprintf(FILE *log, bool debugMode, char *message, ...);

/**
 * @fn getNbCandidat(DataFrame *df)
 * @brief Fonction pour obtenir le nombre de candidats.
 * @param[in] df DataFrame.
 * @return Nombre de candidats.
 */
int getNbCandidat(DataFrame *df);

/**
 * @fn getIdxsCandidats(DataFrame *df, int nb_candidates, int *idxs_candidats)
 * @brief Fonction pour obtenir les index des candidats.
 * @param[in] df DataFrame.
 * @param[in] nb_candidates Nombre de candidats.
 * @param[out] idxs_candidats Index des candidats.
 */
void getIdxsCandidats(DataFrame *df, int nb_candidates, int *idxs_candidats);

/**
 * @fn createVoteResult(int nb_candidates, int nb_voters, int score, char *winner)
 * @brief Fonction pour créer une structure VoteResult.
 * @param[in] nb_candidates Nombre de candidats.
 * @param[in] nb_voters Nombre de votants.
 * @param[in] score Score du vainqueur.
 * @param[in] winner Vainqueur.
 * @return Structure VoteResult.
 */
VoteResult createVoteResult(int nb_candidates, int nb_voters, int score, char *winner);

/**
 * @fn max(int a, int b)
 * @brief Fonction pour obtenir le maximum entre deux entiers.
 * @param[in] a Entier a.
 * @param[in] b Entier b.
 * @return Maximum entre a et b.
 */
int max(int a, int b);

/**
 * @fn min(int a, int b)
 * @brief Fonction pour obtenir le minimum entre deux entiers.
 * @param[in] a Entier a.
 * @param[in] b Entier b.
 * @return Minimum entre a et b.
 */
int min(int a, int b);

#endif // UTILS_H
