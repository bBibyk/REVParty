/**
 * @file condorcet.c
 * @brief //TODO
 * @author Wassim Chenouf
 * @date 31 octobre 2023
 *
 * //TODO
 *
 */

#ifndef CONDORCET_C
#define CONDORCET_C

#include "condorcet.h"
//TODO : includes
VoteResult voteCondorcetMinimax(DataFrame *df, bool duel, FILE *log, bool debugMode){
    VoteResult res;
    res.nb_candidates = 4;
    res.nb_voters = 1000;
    strcpy(res.winner, "Minimax");
    
    return res;
}

VoteResult voteCondorcetSchulze(DataFrame *df, bool duel, FILE *log, bool debugMode){
    VoteResult res;
    res.nb_candidates = 4;
    res.nb_voters = 1000;
    strcpy(res.winner, "Schulze");
    
    return res;
}

VoteResult voteCondorcetPaires(DataFrame *df, bool duel, FILE *log, bool debugMode){
    VoteResult res;
    res.nb_candidates = 4;
    res.nb_voters = 1000;
    strcpy(res.winner, "Paires");
    
    return res;
}
//TODO : code avec commentaires et division en parties

#endif
