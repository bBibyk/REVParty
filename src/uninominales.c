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
char *gagnantUninominalUnTour(DataFrame *df, int *nbVotes, char *columnToSkip)
{
    int numCandidates = df->num_columns - 4;
    int *votes = (int *)malloc(numCandidates * sizeof(int));
    char **candidats = (char **)malloc(numCandidates * sizeof(char *));

    for (int i = 0; i < numCandidates; i++)
    {
        votes[i] = 0;
        candidats[i] = df->columns[i + 4].name;
    }

    Column reponses = dfSelect(df, "Réponse");

    for (int i = 0; i < df->num_rows; i++)
    {
        char response[5];
        sprintf(response, "%d", ((int *)reponses.data)[i]);
        Series row = getRow(df, "Réponse", response);
        for (int j = 0; j < df->num_columns; j++)
        {
            Item item = row.items[j];
            if (columnToSkip != NULL && strcmp(item.label, columnToSkip) == 0)
                continue;

            if (item.type == INT && *(item.value.int_value) == 1)
            {
                votes[j - 4] += 1;
                continue;
            }
        }
    }

    // Recherche du candidat avec le plus grand nombre de votes
    int maxVotes = votes[0];
    int gagnantIndex = 0;

    for (int i = 1; i < numCandidates; i++)
    {
        if (votes[i] > maxVotes)
        {
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
VoteResult voteUninominalUnTour(DataFrame *df, FILE *log, bool debugMode, char *columnToSkip)
{
    VoteResult result;
    int nbVotes;
    strcpy(result.winner, gagnantUninominalUnTour(df, &nbVotes, columnToSkip));

    result.nb_candidates = df->num_columns - 4; // Nombre de candidats
    result.nb_voters = df->num_rows;            // Nombre d'électeurs
    result.score = nbVotes;                     // score du gagnant

    // Ecriture des résultats dans le fichier de log
    if (debugMode)
    {
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

char *preferenceCandidat(DataFrame *df, char *firstCandidate, char *secondCandidate, int *nbVotes)
{

    int numCandidates = 2;
    int votes[2] = {0, 0};

    Column reponses = dfSelect(df, "Réponse");

    for (int i = 0; i < df->num_rows; i++)
    {
        char response[5];
        sprintf(response, "%d", ((int *)reponses.data)[i]);
        Series row = getRow(df, "Réponse", response);
        int scoreFirstCandidate = 0;
        int scoreSecondCandidate = 0;

        for (int j = 0; j < df->num_columns; j++)
        {
            Item item = row.items[j];
            if (strcmp(firstCandidate, item.label) == 0)
                scoreFirstCandidate = selectIntFromSeries(row, firstCandidate);
            else if (strcmp(secondCandidate, item.label) == 0)
                scoreSecondCandidate = selectIntFromSeries(row, secondCandidate);
        }

        if ((scoreFirstCandidate < scoreSecondCandidate) && (scoreFirstCandidate != -1))
            votes[0] += 1;
        else if ((scoreFirstCandidate > scoreSecondCandidate) && (scoreSecondCandidate != -1))
            votes[1] += 1;
        else
            continue;
    }

    if (votes[0] > votes[1])
    {
        *nbVotes = votes[0];
        return firstCandidate;
    }
    else
    {
        *nbVotes = votes[1];
        return secondCandidate;
    }
}

// Fonction pour effectuer un vote uninominal à deux tours
void voteUninominalDeuxTours(DataFrame *df, FILE *log, bool debugMode, VoteResult *firstTourFirstCandidate,VoteResult *firstTourSecondCandidate,  VoteResult *secondTour, bool *majorite)
{

    *firstTourFirstCandidate = voteUninominalUnTour(df, log, debugMode, NULL);

    // Vérifiez si le gagnant du premier tour a obtenu la majorité absolue
    if (firstTourFirstCandidate->score > (float)(df->num_rows - 1) / 2.0)
    {
        *secondTour = *firstTourFirstCandidate;
        *majorite = true;
    }
    else
    {
        // Création d'un DataFrame temporaire avec seulement les deux candidats les mieux placés
        char *firstCandidate = firstTourFirstCandidate->winner;
        *firstTourSecondCandidate = voteUninominalUnTour(df, log, debugMode, firstCandidate);
        char *secondCandidate = firstTourSecondCandidate.winner;
        int nbVotes;

        secondTour->nb_candidates = 2;
        secondTour->nb_voters = df->num_rows;
        strcpy(secondTour->winner, preferenceCandidat(df, firstCandidate, secondCandidate, &nbVotes));
        secondTour->score = nbVotes;
    }
    
    // Ecriture des résultats du deuxième tour dans le fichier de log
    if (debugMode)
    {
        fprintf(log, "\nRésultats du deuxième tour :\n");
        fprintf(log, "Gagnant : %s\n", secondTour->winner);
        fprintf(log, "Nombre de candidats : %d\n", secondTour->nb_candidates);
        fprintf(log, "Nombre d'électeurs : %d\n", secondTour->nb_voters);
        fprintf(log, "Score : %d\n", secondTour->score);
        fprintf(log, "\n");
    }
}

#endif