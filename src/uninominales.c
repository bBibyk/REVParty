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
    int numCandidates = df->num_columns - 4;
    int *votes = (int *)malloc(numCandidates * sizeof(int));
    char **candidats = (char **)malloc(numCandidates * sizeof(char *));
    for (int i = 0; i < numCandidates; i++) {
        votes[i] = 0;
        candidats[i] = df->columns[i+4].name;
    }
    Column reponses = dfSelect(df, "Réponse");
    for (int i = 0; i < df->num_rows; i++){
        Series row = getRow(df, "Réponse", (int*)reponses.data[i]);
        for (int j = 0; j < df->num_columns; j++){
            Item item = row.items[j];
            if (item.type == INT && item.value == 1){
                votes[j-4] += 1;
                break;
            }
        }   
    }


    // Recherche du candidat avec le plus grand nombre de votes
    int maxVotes = votes[0];
    int gagnantIndex = 0;


    for (int i = 1; i < numCandidates; i++) {
        if (votes[i] > maxVotes) {
            maxVotes = votes[i];
            gagnantIndex = i;
        }
    }

    // Enregistrement du nombre de votes pour le gagnant
    *nbVotes = maxVotes;

    free(votes);

    // Retour du nom du candidat gagnant
    return candidats[gagnantIndex];
    }

// Fonction pour effectuer un vote uninominal à un tour
VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode) {
    VoteResult result;
    int *nbVotes;
    strcpy(result.winner,gagnantUninominalUnTour(df, nbVotes));

    result.nb_candidates = df->num_columns - 4; // Nombre de candidats
    result.nb_voters = df->num_rows; // Nombre d'électeurs
    result.score = *nbVotes; // score du gagnant

    // Affichage des résultats si le mode débogage est activé
    if (debugMode) {
        printf("Résultats du vote uninominal à un tour :\n");
        printf("Gagnant : %s\n", result.winner);
        printf("Nombre de candidats : %d\n", result.nb_candidates);
        printf("Nombre d'électeurs : %d\n", result.nb_voters);
        printf("Score : %d\n", result.score);
    }
    // Ecriture des résultats dans le fichier de log
    if (log != NULL) {
        fprintf(log, "Résultats du vote uninominal à un tour :\n");
        fprintf(log, "Gagnant : %s\n", result.winner);
        fprintf(log, "Nombre de candidats : %d\n", result.nb_candidates);
        fprintf(log, "Nombre d'électeurs : %d\n", result.nb_voters);
        fprintf(log, "Score : %d\n", result.score);
        fprintf(log, "\n");
    }

    return result;
}






///====================================================================================
///                                 UNINOMINAL 2 TOUR
///====================================================================================


// Fonction auxiliaire pour trouver le deuxième candidat le mieux placé
char *deuxiemeCandidat(DataFrame *df, char *firstCandidate) {
}


// Fonction auxiliaire pour créer un DataFrame temporaire pour le deuxième tour
DataFrame *createSecondTourDataFrame(DataFrame *df, char *firstCandidate, char *secondCandidate) {
    char *selectedColumns[] = {};
    DataFrame *secondTour = df;

    return secondTour;

}


// Fonction pour effectuer un vote uninominal à deux tours
void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTour, VoteResult *secondTour) {

    *firstTour = voteUninominalUnTour(df, log, debugMode);

    // Vérifiez si le gagnant du premier tour a obtenu la majorité absolue
    if (firstTour->score > (float)(df->num_rows-1) / 2.0) {
        *secondTour = *firstTour;
    } 
    else {
        // Création d'un DataFrame temporaire avec seulement les deux candidats les mieux placés
        char *firstCandidate = firstTour->winner;
        char *secondCandidate = deuxiemeCandidat(df, firstTour->winner);

        /*
        DataFrame *secondTour = createSecondTourDataFrame(df, firstCandidate, secondCandidate);

        *secondTour = voteUninominalUnTour(secondTour, log, debugMode);

        // Libération de la mémoire du DataFrame temporaire
        freeDataFrame(secondTour);  
    }

    */
    // Affichage des résultats du deuxième tour si le mode débogage est activé
     if (debugMode) {
        printf("\nRésultats du premier tour :\n");
        printf("Gagnant : %s\n", firstTour->winner);
        printf("Nombre de candidats : %d\n", firstTour->nb_candidates);
        printf("Nombre d'électeurs : %d\n", firstTour->nb_voters);
        printf("Score : %d\n", firstTour->score);
    }

    // Ecriture des résultats du deuxième tour dans le fichier de log
    if (log != NULL) {
        fprintf(log, "\nRésultats du premier tour :\n");
        fprintf(log, "Gagnant : %s\n", firstTour->winner);
        fprintf(log, "Nombre de candidats : %d\n", firstTour->nb_candidates);
        fprintf(log, "Nombre d'électeurs : %d\n", firstTour->nb_voters);
        fprintf(log, "Score : %d\n", firstTour->score);
        fprintf(log, "\n");
    }

    // Affichage des résultats du deuxième tour si le mode débogage est activé
    if (debugMode) {
        printf("\nRésultats du deuxième tour :\n");
        printf("Gagnant : %s\n", secondTour->winner);
        printf("Nombre de candidats : %d\n", secondTour->nb_candidates);
        printf("Nombre d'électeurs : %d\n", secondTour->nb_voters);
        printf("Score : %d\n", secondTour->score);
    }

    // Ecriture des résultats du deuxième tour dans le fichier de log
    if (log != NULL) {
        fprintf(log, "\nRésultats du deuxième tour :\n");
        fprintf(log, "Gagnant : %s\n", secondTour->winner);
        fprintf(log, "Nombre de candidats : %d\n", secondTour->nb_candidates);
        fprintf(log, "Nombre d'électeurs : %d\n", secondTour->nb_voters);
        fprintf(log, "Score : %d\n", secondTour->score);
        fprintf(log, "\n");
    }
}

#endif