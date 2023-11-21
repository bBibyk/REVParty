#ifndef UNINOMINALES_H
#define UNINOMINALES_H

#include "lecture_csv.h"

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
 * @fn VoteResult voteUninominalUnTour(DataFrame *df);
 * @brief Fonction pour effectuer un vote uninominal à un tour.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @return Structure VoteResult contenant les résultats du vote uninominal à un tour.
 */
VoteResult voteUninominalUnTour(DataFrame *df);

/**
 * @fn VoteResult voteUninominalDeuxTours(DataFrame *df);
 * @brief Fonction pour effectuer un vote uninominal à deux tours.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @return Structure VoteResult contenant les résultats du vote uninominal à deux tours.
 */
VoteResult voteUninominalDeuxTours(DataFrame *df);

#endif
