/**
 * @file condorcet.h
 * @brief Gestion de méthodes de vote Condorcet selon plusieurs façons de résolution du cas d'égalité.
 * @author Bibyk Bogdan
 * @date 30 novembre 2023
 *
 * //TODO
 *
 */

#ifndef CONDORCET_H
#define CONDORCET_H

#include "lecture_csv.h"
#include "utils.h"
#include "graph.h"
#include <stdbool.h>


/**
 * @fn condorcetMinimax(DataFrame df, bool duel, FILE *log, bool debugMode);
 * @brief Fonction pour effectuer un vote Condorcet utilisant la méthode minimax.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] duel Indique si le DataFrame est en un fichier de duels.
 * @param[in] log Pointeur vers le fichier journal des sorties.
 * @param[in] dubugMode Indique si le mode débogage est activé.
 * @return Structure VoteResult contenant les résultats du vote Condorcet utilisant la méthode minimax.
 */
VoteResult voteCondorcetMinimax(DataFrame *df, bool duel, FILE *log, bool debugMode);

/**
 * @fn condorcetMinimax(DataFrame df, bool duel, FILE *log, bool debugMode);
 * @brief Fonction pour effectuer un vote Condorcet utilisant la méthode Schulze.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] duel Indique si le DataFrame est en un fichier de duels.
 * @param[in] log Pointeur vers le fichier journal des sorties.
 * @param[in] dubugMode Indique si le mode débogage est activé.
 * @return Structure VoteResult contenant les résultats du vote Condorcet utilisant la méthode Schulze.
 */
VoteResult voteCondorcetSchulze(DataFrame *df, bool duel, FILE *log, bool debugMode);

/**
 * @fn condorcetMinimax(DataFrame df, bool duel, FILE *log, bool debugMode);
 * @brief Fonction pour effectuer un vote Condorcet utilisant la méthode des paires.
 * @param[in] df Pointeur vers le DataFrame contenant les résultats du vote.
 * @param[in] duel Indique si le DataFrame est en un fichier de duels.
 * @param[in] log Pointeur vers le fichier journal des sorties.
 * @param[in] dubugMode Indique si le mode débogage est activé.
 * @return Structure VoteResult contenant les résultats du vote Condorcet utilisant la méthode des paires.
 */
VoteResult voteCondorcetPaires(DataFrame *df, bool duel, FILE *log, bool debugMode);

#endif