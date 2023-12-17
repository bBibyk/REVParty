/**
 * @file utils.c
 * @brief Fonctions et structures utiles pour les différents systèmes de votes.
 * @authors Bibyk Bogdan
 * @date 11 novembre 2023
 *
 * Fichier avec les utiles pour les différents systèmes de votes.
 *
 */

#ifndef UTILS_C
#define UTILS_C

#include "utils.h"
#include <stdarg.h>

/////////////////////
// -- Fonctions -- //
/////////////////////

/**
 * @fn void printResult(VoteResult result, char *method, int tour)
 * @brief Fonction pour afficher les résultats d'un vote.
 * @param[in] result Structure contenant les données du vote.
 * @param[in] method Mode de scrutin.
 * @param[in] tour Tour du vote, si uninominal.
 *
 * @note L'affichage est base sur le mode de scrutin.
 */
void printResult(VoteResult result, char *method, int tour)
{
    printf("Mode de scrutin : ");

    if (strcmp(method, "uni1") == 0)
    {
        printf("uninominal à un tour, tour %d", tour);
    }
    else if (strcmp(method, "uni2") == 0)
    {
        printf("uninominal à deux tours, tour %d", tour);
    }
    else if (strcmp(method, "cm") == 0)
    {
        printf("Condorcet minimax");
    }
    else if (strcmp(method, "cp") == 0)
    {
        printf("Condorcet paires");
    }
    else if (strcmp(method, "cs") == 0)
    {
        printf("Condorcet Schulze");
    }
    else if (strcmp(method, "jm") == 0)
    {
        printf("jugement majoritaire");
    }

    printf(", %d candidats, %d votants, vainqueur = %s", result.nb_candidates, result.nb_voters, result.winner);

    if (strcmp(method, "uni1") == 0 || strcmp(method, "uni2") == 0)
    {
        float score_percentage = (result.score / result.nb_voters) * 100;
        printf(", score = %.2f%%", score_percentage);
    }

    printf("\n");
}

/**
 * @fn void logprintf(FILE *log, bool debugMode, char *message, ...)
 * @brief Fonction pour afficher les messages de debug.
 * @param[in] log Fichier de log.
 * @param[in] debugMode Mode de debug.
 * @param[in] message Message à afficher.
 * @param[in] ... Arguments du message.
 */
void logprintf(FILE *log, bool debugMode, char *message, ...)
{
    if (debugMode)
    {
        va_list args;
        va_start(args, message);
        vfprintf(log, message, args);
        va_end(args);
    }
}

/**
 * @fn getNbCandidat(DataFrame *df)
 * @brief Fonction pour obtenir le nombre de candidats.
 * @param[in] df DataFrame.
 * @return Nombre de candidats.
 */
int getNbCandidat(DataFrame *df)
{
    Column *columns = df->columns;
    int num_columns = df->num_columns;
    int nb_candidates = 0;
    for (int i = 1; i < num_columns; i++)
    {
        if (columns[i].ctype == INT)
        {
            nb_candidates++;
        }
    }
    return nb_candidates;
}

/**
 * @fn getIdxsCandidats(DataFrame *df, int nb_candidates, int *idxs_candidats)
 * @brief Fonction pour obtenir les index des candidats.
 * @param[in] df DataFrame.
 * @param[in] nb_candidates Nombre de candidats.
 * @param[out] idxs_candidats Index des candidats.
 */
void getIdxsCandidats(DataFrame *df, int nb_candidates, int *idxs_candidats)
{
    for (int i = 0; i < nb_candidates; i++)
    {
        idxs_candidats[i] = df->num_columns - nb_candidates + i;
    }
}

/**
 * @fn createVoteResult(int nb_candidates, int nb_voters, int score, char *winner)
 * @brief Fonction pour créer une structure VoteResult.
 * @param[in] nb_candidates Nombre de candidats.
 * @param[in] nb_voters Nombre de votants.
 * @param[in] score Score du vainqueur.
 * @param[in] winner Vainqueur.
 * @return Structure VoteResult.
 */
VoteResult createVoteResult(int nb_candidates, int nb_voters, int score, char *winner)
{
    VoteResult res;
    res.nb_candidates = nb_candidates;
    res.nb_voters = nb_voters;
    res.score = score;
    strcpy(res.winner, winner);
    return res;
}

/**
 * @fn max(int a, int b)
 * @brief Fonction pour obtenir le maximum entre deux entiers.
 * @param[in] a Entier a.
 * @param[in] b Entier b.
 * @return Maximum entre a et b.
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * @fn min(int a, int b)
 * @brief Fonction pour obtenir le minimum entre deux entiers.
 * @param[in] a Entier a.
 * @param[in] b Entier b.
 * @return Minimum entre a et b.
 */
int min(int a, int b)
{
    return (a < b) ? a : b;
}

#endif // UTILS_C
