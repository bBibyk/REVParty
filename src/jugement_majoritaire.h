#ifndef JUGEMENT_MAJORITAIRE_H
#define JUGEMENT_MAJORITAIRE_H

#include "lecture_csv.h"
#include "utils.h"

/**
 * @file jugement_majoritaire.h
 * @brief Gestion de la méthode de jugement majoritaire.
 * @author Pigani Emanuele
 * @date 31 octobre 2023
 *
 * Ce fichier d'en-tête définit les fonctions nécessaires pour effectuer un vote
 * selon la méthode de jugement majoritaire et stocker les résultats dans la structure VoteResult.
 */

/**
 * @fn VoteResult voteJugementMajoritaire(DataFrame *df);
 * @brief Fonction pour effectuer un vote selon la méthode de jugement majoritaire.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @return Structure VoteResult contenant les résultats du vote selon la méthode de jugement majoritaire.
 */
VoteResult voteJugementMajoritaire(DataFrame *df);

#endif
