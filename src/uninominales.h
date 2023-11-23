#ifndef UNINOMINALES_H
#define UNINOMINALES_H

#include "lecture_csv.h"
#include "utils.h"
#include <stdbool.h>

/**
 * @file uninominales.h
 * @brief Gestion des méthodes de vote uninominales à un et deux tours.
 * @author Pigani Emanuele
 * @date 31 octobre 2023
 *
 * Ce fichier d'en-tête définit les fonctions nécessaires pour effectuer des votes uninominaux
 * à un et deux tours et stocker les résultats dans la structure VoteResult.
 */

/**
 * @fn VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode);
 * @brief Fonction pour effectuer un vote uninominal à un tour.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] log Pointeur vers le fichier de test des sorties.
 * @param[in] debugMode Indique si le mode débogage est activé.
 * @return Structure VoteResult contenant les résultats du vote uninominal à un tour.
 */
VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode);

/**
 * @fn void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTour, VoteResult *secondTour);
 * @brief Fonction pour effectuer un vote uninominal à deux tours.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] log Pointeur vers le fichier de test des sorties.
 * @param[in] debugMode Indique si le mode débogage est activé.
 * @param[out] firstTour Résultats du premier tour de vote.
 * @param[out] secondTour Résultats du deuxième tour de vote.
 */
void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTour, VoteResult *secondTour);

#endif