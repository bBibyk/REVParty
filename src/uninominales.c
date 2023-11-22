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
#include "lecture_csv.h"
#include <string.h>
//TODO : includes

VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode){
    VoteResult res;

    res.nb_candidates = 10;
    res.nb_voters = 100;
    res.score = 51;
    strcpy(res.winner, "Pigani");

    return res;
}

//TODO : code avec commentaires et division en parties

#endif
