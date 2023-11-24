#ifndef UNINOMINALES_C
#define UNINOMINALES_C

#include "uninominales.h"

/**
 * @file uninominales.c
 * @brief Implémentation des méthodes de vote uninominales à un et deux tours, avec stockage des résultats dans une structure VoteResult.
 * @author Pigani Emanuele
 * @date 31 octobre 2023
 *
 * Ce fichier contient l'implémentation des méthodes de vote uninominales à un et deux tours.
 * Les résultats du vote sont stockés dans une structure VoteResult.
 */



///====================================================================================
///                                 UNINOMINAL 1 TOUR
///====================================================================================


// Fonction auxiliaire pour trouver le gagnant d'un vote uninominal à un tour
char *gagnantUninominalUnTour(DataFrame *df, int *nbVotes) {
    }

// Fonction pour effectuer un vote uninominal à un tour
VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode) {
    VoteResult result;
    int nbVotes;
    strcpy(result.winner,gagnantUninominalUnTour(df));

    result.nbCandidates = df->num_columns - 1; // Nombre de candidats
    result.nbVoters = df->num_rows; // Nombre d'électeurs
    result.score = nbVotes; // score du gagnant

    // Affichage des résultats si le mode débogage est activé
    if (debugMode) {
        printf("Résultats du vote uninominal à un tour :\n");
        printf("Gagnant : %s\n", result.winner);
        printf("Nombre de candidats : %d\n", result.nbCandidates);
        printf("Nombre d'électeurs : %d\n", result.nbVoters);
        printf("Score : %f\n", result.score);
    }
    // Ecriture des résultats dans le fichier de log
    if (log != NULL) {
        fprintf(log, "Résultats du vote uninominal à un tour :\n");
        fprintf(log, "Gagnant : %s\n", result.winner);
        fprintf(log, "Nombre de candidats : %d\n", result.nbCandidates);
        fprintf(log, "Nombre d'électeurs : %d\n", result.nbVoters);
        fprintf(log, "Score : %f\n", result.score);
        fprintf(log, "\n");
    }

    return result;
}






///====================================================================================
///                                 UNINOMINAL 2 TOUR
///====================================================================================


// Fonction auxiliaire pour trouver le deuxième candidat le mieux placé
char *findSecondCandidate(DataFrame *df, char *firstCandidate) {
}


// Fonction auxiliaire pour créer un DataFrame temporaire pour le deuxième tour
DataFrame *createSecondTourDataFrame(DataFrame *df, char *firstCandidate, char *secondCandidate) {

}
// Fonction pour effectuer un vote uninominal à deux tours
void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTour, VoteResult *secondTour) {

    *firstTour = voteUninominalUnTour(df, log, debugMode);

    // Vérifiez si le gagnant du premier tour a obtenu la majorité absolue
    if (firstTour->score > (float)(df->num_rows) / 2.0) {
        *secondTour = *firstTour;
    } 
    else {
        // Création d'un DataFrame temporaire avec seulement les deux candidats les mieux placés
        char *firstCandidate = firstTour->winner;
        char *secondCandidate = deuxiemeCandidat(df, firstTour->winner);
        DataFrame *secondTour = createSecondTourDataFrame(df, firstCandidate, secondCandidate);

        *secondTour = voteUninominalUnTour(secondTourDF, log, debugMode);

        // Libération de la mémoire du DataFrame temporaire (FONCTION A IMPLEMENTER DANS LECTURE_CSV)
        freeDataFrame(secondTour);  
    }


    // Affichage des résultats du deuxième tour si le mode débogage est activé
     if (debugMode) {
        printf("\nRésultats du premier tour :\n");
        printf("Gagnant : %s\n", firstTour->winner);
        printf("Nombre de candidats : %d\n", firstTour->nbCandidates);
        printf("Nombre d'électeurs : %d\n", firstTour->nbVoters);
        printf("Score : %f\n", firstTour->score);
    }

    // Ecriture des résultats du deuxième tour dans le fichier de log
    if (log != NULL) {
        fprintf(log, "\nRésultats du premier tour :\n");
        fprintf(log, "Gagnant : %s\n", firstTour->winner);
        fprintf(log, "Nombre de candidats : %d\n", firstTour->nbCandidates);
        fprintf(log, "Nombre d'électeurs : %d\n", firstTour->nbVoters);
        fprintf(log, "Score : %f\n", firstTour->score);
        fprintf(log, "\n");
    }

    // Affichage des résultats du deuxième tour si le mode débogage est activé
    if (debugMode) {
        printf("\nRésultats du deuxième tour :\n");
        printf("Gagnant : %s\n", secondTour->winner);
        printf("Nombre de candidats : %d\n", secondTour->nbCandidates);
        printf("Nombre d'électeurs : %d\n", secondTour->nbVoters);
        printf("Score : %f\n", secondTour->score);
    }

    // Ecriture des résultats du deuxième tour dans le fichier de log
    if (log != NULL) {
        fprintf(log, "\nRésultats du deuxième tour :\n");
        fprintf(log, "Gagnant : %s\n", secondTour->winner);
        fprintf(log, "Nombre de candidats : %d\n", secondTour->nbCandidates);
        fprintf(log, "Nombre d'électeurs : %d\n", secondTour->nbVoters);
        fprintf(log, "Score : %f\n", secondTour->score);
        fprintf(log, "\n");
    }
}

#endif