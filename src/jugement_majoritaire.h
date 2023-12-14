/**
 * @file jugement_majoritaire.h
 * @brief Gestion de la méthode de jugement majoritaire.
 * @author Pigani Emanuele, Bibyk Bogdan
 * @date 01 décembre 2023
 *
 * Ce fichier d'en-tête définit les fonctions nécessaires pour effectuer un vote
 * selon la méthode de jugement majoritaire et stocker les résultats dans la structure VoteResult.
 */

#ifndef JUGEMENT_MAJORITAIRE_H
#define JUGEMENT_MAJORITAIRE_H

#include "lecture_csv.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @fn VoteResult voteJugementMajoritaire(DataFrame *df);
 * @brief Fonction pour effectuer un vote selon la méthode de jugement majoritaire.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] duel Indique si le DataFrame est en un fichier de duels.
 * @param[in] log Pointeur vers le fichier journal des sorties.
 * @param[in] debugMode Indique si le mode débogage est activé.
 * @return Structure VoteResult contenant les résultats du vote selon la méthode de jugement majoritaire.
 */
VoteResult voteJugementMajoritaire(DataFrame *df, FILE *log, bool debugMode);

#endif