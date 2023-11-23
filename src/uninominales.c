/**
 * @file uninominales.c
 * @brief //TODO
 * @author Pigani Emanuele
 * @date 31 octobre 2023
 *
 * //TODO
 *
 */

#ifndef UNINOMINALES_C
#define UNINOMINALES_C

#include "uninominales.h"
//TODO : includes
VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode){
    VoteResult res;

    res.nb_candidates = 10;
    res.nb_voters = 100;
    res.score = 51;
    strcpy(res.winner, "Pigani");

    return res;
}

void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTour, VoteResult *secondTour){

    firstTour->nb_candidates = 1;
    firstTour->nb_voters = 1;
    firstTour->score = 1;
    strcpy(firstTour->winner, "P1");

    secondTour->nb_candidates = 2;
    secondTour->nb_voters = 2;
    secondTour->score = 2;
    strcpy(secondTour->winner, "P2");
}
//TODO : code avec commentaires et division en parties

#endif