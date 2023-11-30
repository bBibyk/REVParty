/**
 * @file jugement_majoritaire.c
 * @brief //TODO
 * @author Pigani Emanuele
 * @date 31 octobre 2023
 *
 * //TODO
 *
 */

#ifndef JUGEMENT_MAJORITAIRE_C
#define JUGEMENT_MAJORITAIRE_C

#include "jugement_majoritaire.h"
//TODO : includes

VoteResult voteJugementMajoritaire(DataFrame *df, bool duel, FILE *log, bool debugMode){
    VoteResult res;
    res.nb_candidates = 4;
    res.nb_voters = 1000;
    strcpy(res.winner, "Mamoon");
    
    return res;
}
//TODO : code avec commentaires et division en parties

#endif
