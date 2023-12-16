/**
 * @file condorcet.c
 * @brief Méthodes de vote Condorcet.
 * @author Essengue Matis
 * @date 16 décembre 2023
 *
 * Ce fichier contient les implémentations des méthodes de vote Condorcet.
 * Les méthodes implémentées sont :
 * - Minimax
 * - Des Paires
 * - Schulze
 *
 */

#ifndef CONDORCET_C
#define CONDORCET_C

#include "condorcet.h"

//////////////////////////////
// -- Fonctions Communes -- //
//////////////////////////////

/**
 * @fn int calculerScoreDuel(DataFrame *df, int idx_candidat, int idx_adversaire, char *candidat, char *adversaire, FILE *log, bool debugMode)
 * @brief Calcule le score d'un duel entre deux candidats.
 * @param df Le DataFrame contenant les données de vote.
 * @param[in] candidat Le nom du candidat.
 * @param[in] adversaire Le nom de l'adversaire.
 * @param[in] log Le fichier de log.
 * @param[in] debugMode Le mode debug.
 * @return Le score du duel.
 */
static int calculerScoreDuel(DataFrame *df, char *candidat, char *adversaire, FILE *log, bool debugMode)
{
    int score = 0;
    for (int k = 0; k < df->num_rows; k++)
    {
        char response[100];
        sprintf(response, "%d", ((int *)df->columns[0].data)[k]);
        Series row = getRow(df, df->columns[0].name, response);
        int vote_candidat = selectIntFromSeries(row, candidat);     // on récupère le rang du candidat
        int vote_adversaire = selectIntFromSeries(row, adversaire); // on récupère le rang de l'adversaire

        // Si le candidat a un rang plus petit que l'adversaire, il gagne un point
        if ((vote_candidat < vote_adversaire && vote_candidat != -1) || (vote_adversaire == -1 && vote_candidat != -1))
            score++;
        // Sinon, il perd un point
        else if ((vote_candidat > vote_adversaire && vote_adversaire != -1) || (vote_candidat == -1 && vote_adversaire != -1))
            score--;
    }
    return score;
}

///////////////////////////
// -- Méthode MiniMax -- //
///////////////////////////

/**
 * @fn void updateWinner(char **winner, int *winner_score, char *candidat, int candidat_score)
 * @brief Met à jour le vainqueur.
 * @param[in] winner Le vainqueur actuel.
 * @param[in] winner_score Le score du vainqueur actuel.
 * @param[in] candidat Le candidat à comparer.
 * @param[in] candidat_score Le score du candidat à comparer.
 *
 * Si le score du candidat est supérieur au score du vainqueur, le vainqueur est mis à jour.
 */
static void updateWinner(char **winner, int *winner_score, char *candidat, int candidat_score)
{
    if (*winner == NULL || candidat_score > *winner_score)
    {
        *winner = malloc(sizeof(char) * 50);
        strcpy(*winner, candidat);
        *winner_score = candidat_score;
    }
}

/**
 * @fn void affronter(int j, int *idxs_candidats, DataFrame *df, FILE *log, bool debugMode, int idx_candidat, char *candidat, int *mini)
 * @brief Affronte deux candidats.
 * @param[in] j L'indice du candidat à affronter.
 * @param[in] idxs_candidats Les indices des candidats.
 * @param[in] df Le DataFrame contenant les données de vote.
 * @param[in] log Le fichier de log.
 * @param[in] debugMode Le mode debug.
 * @param[in] candidat Le nom du candidat.
 * @param[in] mini Le score minimum.
 *
 * Fait s'affronter deux candidats et met à jour le score minimum.
 */
static void affronter(int j, int *idxs_candidats, DataFrame *df, FILE *log, bool debugMode, char *candidat, int *mini)
{
    int idx_adversaire = idxs_candidats[j];
    char *adversaire = df->columns[idx_adversaire].name;
    logprintf(log, debugMode, "\tduel vs %s: ", adversaire);

    // On calcule le score du duel et on met à jour le score minimum si besoin
    int score = calculerScoreDuel(df, candidat, adversaire, log, debugMode);
    if (score < *mini)
        *mini = score;
    logprintf(log, debugMode, "%d\n", score);
}

/**
 * @fn char *trouverMiniMaxDuel(DataFrame *df, FILE *log, bool debugMode, int *score)
 * @brief Trouve le vainqueur de la méthode MiniMax pour un duel.
 * @param[in] df Le DataFrame contenant les données de vote.
 * @param[in] log Le fichier de log.
 * @param[in] debugMode Le mode debug.
 * @param[out] score Le score du vainqueur.
 * @return Le nom du vainqueur.
 */
static char *trouverMiniMaxDuel(DataFrame *df, FILE *log, bool debugMode, int *score)
{
    logprintf(log, debugMode, "METHODE MINI MAX DUEL:\n");
    char *winner = NULL;
    int winner_score = 0;
    for (int i = 0; i < df->num_columns; i++)
    {
        char *candidat = df->columns[i].name;
        int score_candidat = 0;
        Column col = dfSelect(df, candidat);
        for (int j = 0; j < df->num_columns; j++)
        {
            if (i != j)
            {
                char *adversaire = df->columns[j].name;
                Column col = dfSelect(df, adversaire);
                int score_duel = ((int *)col.data)[i];
                if (score_candidat == 0 || score_duel < score_candidat)
                    score_candidat = score_duel;
            }
        }
        logprintf(log, debugMode, "%s: %d\n", candidat, score_candidat);
        updateWinner(&winner, &winner_score, candidat, score_candidat);
    }
    logprintf(log, debugMode, "Vainqueur MINI MAX DUEL: %s, avec un score de %d\n\n", winner, winner_score);
    return winner;
}

/**
 * @fn char *trouverMiniMax(DataFrame *df, int *idxs_candidats, int nb_candidates, FILE *log, bool debugMode, int *score)
 * @brief Trouve le vainqueur de la méthode MiniMax.
 * @param[in] df Le DataFrame contenant les données de vote.
 * @param[in] idxs_candidats Les indices des candidats.
 * @param[in] nb_candidates Le nombre de candidats.
 * @param[in] log Le fichier de log.
 * @param[in] debugMode Le mode debug.
 * @param[out] score Le score du vainqueur.
 * @return Le nom du vainqueur.
 */
static char *trouverMiniMax(DataFrame *df, int *idxs_candidats, int nb_candidates, FILE *log, bool debugMode, int *score, bool duel)
{
    if (duel)
        return trouverMiniMaxDuel(df, log, debugMode, score);

    logprintf(log, debugMode, "METHODE MINI MAX CLASSIQUE:\n");
    char *winner = NULL;
    int winner_score = 0;
    for (int i = 0; i < nb_candidates; i++)
    {
        // On récupère le candidat et on initialise le score minimum à la taille du DataFrame
        // En faisant ça on s'assure que n'importe quel score sera inférieur au score minimum initial
        int idx_candidat = idxs_candidats[i];
        int mini = df->num_rows;
        char *candidat = df->columns[idx_candidat].name;
        logprintf(log, debugMode, "Candidat %s:\n", candidat);

        // On fait affronter le candidat à tous les autres candidats pour trouver son score minimum
        for (int j = 0; j < nb_candidates; j++)
        {
            if (i != j)
                affronter(j, idxs_candidats, df, log, debugMode, candidat, &mini);
        }
        logprintf(log, debugMode, "\tmini: %d\n", mini);

        // On met à jour le vainqueur si necessaire
        updateWinner(&winner, &winner_score, candidat, mini);
        logprintf(log, debugMode, "\n");
    }
    *score = winner_score;
    logprintf(log, debugMode, "Vainqueur MINI MAX CLASSIQUE: %s, avec un score de %d\n\n", winner, winner_score);
    return winner;
}

/**
 * @fn VoteResult voteCondorcetMinimax(DataFrame *df, bool duel, FILE *log, bool debugMode)
 * @brief Vote Condorcet par la méthode MiniMax.
 * @param[in] df Le DataFrame contenant les données de vote.
 * @param[in] duel Si vrai, le df est une matrice de duel.
 * @param[in] log Le fichier de log.
 * @param[in] debugMode Le mode debug.
 * @return Le résultat du vote.
 */
VoteResult voteCondorcetMinimax(DataFrame *df, bool duel, FILE *log, bool debugMode)
{
    // On récupère les candidats
    int nb_candidates = getNbCandidat(df);
    int idxs_candidats[nb_candidates];
    char *index_name = df->columns[0].name;
    getIdxsCandidats(df, nb_candidates, idxs_candidats);

    // On trouve le vainqueur et son score
    int score;
    char *minimax_vainqueur = trouverMiniMax(df, idxs_candidats, nb_candidates, log, debugMode, &score, duel);

    // On crée le résultat du vote et on l'affiche
    VoteResult res = createVoteResult(nb_candidates, df->num_rows, score, minimax_vainqueur);
    printResult(res, "cm", 1);

    // On libère la mémoire et on retourne le résultat
    free(minimax_vainqueur);
    return res;
}

//////////////////////////////
// -- Méthode Des Paires -- //
//////////////////////////////

static void addEdge(Graph *graph, int j, int *idxs_candidats, DataFrame *df, FILE *log, bool debugMode, int idx_candidat, char *candidat)
{
    int idx_adversaire = idxs_candidats[j];
    char *adversaire = df->columns[idx_adversaire].name;
    int score = calculerScoreDuel(df, candidat, adversaire, log, debugMode);
    if (score > 0)
        setEdge(graph, candidat, adversaire, score);
}

static Graph *initGraphNodeFromDf(DataFrame *df, int *idxs_candidats, int nb_candidates)
{
    Graph *graph = createGraph();
    for (int i = 0; i < nb_candidates; i++)
    {
        int idx_candidat = idxs_candidats[i];
        char *candidat = df->columns[idx_candidat].name;
        addNode(graph, candidat);
    }
    return graph;
}

static Graph *fillGraphFromDf(DataFrame *df, int *idxs_candidats, int nb_candidates, FILE *log, bool debugMode)
{
    char *index_name = df->columns[0].name;
    Graph *graph = initGraphNodeFromDf(df, idxs_candidats, nb_candidates);

    for (int i = 0; i < nb_candidates; i++)
    {
        int idx_candidat = idxs_candidats[i];
        char *candidat = df->columns[idx_candidat].name;
        for (int j = 0; j < nb_candidates; j++)
        {
            if (i != j)
                addEdge(graph, j, idxs_candidats, df, log, debugMode, idx_candidat, candidat);
        }
    }
    return graph;
}

VoteResult voteCondorcetPaires(DataFrame *df, bool duel, FILE *log, bool debugMode)
{
    int nb_candidates = getNbCandidat(df);
    char *candidates_names[nb_candidates];
    int idxs_candidats[nb_candidates];
    getIdxsCandidats(df, nb_candidates, idxs_candidats);
    for (int i = 0; i < nb_candidates; i++)
        candidates_names[i] = df->columns[idxs_candidats[i]].name;

    Graph *graph = fillGraphFromDf(df, idxs_candidats, nb_candidates, log, debugMode);
    printGraph(graph, log);

    int *sortedValues;
    int *coordinates;
    sortedMatrixValues(graph, &sortedValues, &coordinates);

    Graph *uncycledGraph = initGraphNodeFromDf(df, idxs_candidats, nb_candidates);
    for (int i = 0; i < graph->nb_nodes * graph->nb_nodes; i++)
    {
        int value = sortedValues[i];
        if (value == 0)
            break;

        char *from = candidates_names[coordinates[i * 2]];
        char *to = candidates_names[coordinates[i * 2 + 1]];
        setEdge(uncycledGraph, from, to, value);
        fprintf(log, "\n");
        printGraph(uncycledGraph, log);
        logprintf(log, debugMode, "Ajout de %s -> %s\n", from, to);
        if (isCycled(uncycledGraph))
        {
            logprintf(log, debugMode, "!! cycle detecté entre %s et %s !!\n", from, to);
            removeEdge(uncycledGraph, from, to);
            printGraph(uncycledGraph, log);
        }
    }

    for (int i = 0; i < nb_candidates; i++)
    {
        char *candidat = candidates_names[i];
        if (!isDominant(uncycledGraph, candidat))
            removeNode(uncycledGraph, candidat);
    }
    char *winner = uncycledGraph->nodes[0];

    VoteResult res;
    res.nb_candidates = nb_candidates;
    res.nb_voters = df->num_rows;
    strcpy(res.winner, winner);
    printResult(res, "cp", 1);

    return res;
}

//////////////////////////////
// -- Méthode De Schulze -- //
//////////////////////////////

// -- Méthode de Schulze -- //

static void calculerCheminsFort(DataFrame *df, int **chemins, int *idxs_candidats, int nb_candidates, FILE *log, bool debugMode)
{
    // Initialiser les chemins à 0
    for (int i = 0; i < nb_candidates; i++)
        for (int j = 0; j < nb_candidates; j++)
            chemins[i][j] = (i == j) ? 0 : -1;

    // Calculer la force des chemins directs
    for (int i = 0; i < nb_candidates; i++)
    {
        for (int j = 0; j < nb_candidates; j++)
        {
            if (i != j)
            {
                int score = calculerScoreDuel(df, df->columns[idxs_candidats[i]].name, df->columns[idxs_candidats[j]].name, log, debugMode);
                chemins[i][j] = score;
            }
        }
    }

    // Calculer les chemins les plus forts pour chaque paire de candidats
    for (int k = 0; k < nb_candidates; k++)
        for (int i = 0; i < nb_candidates; i++)
            for (int j = 0; j < nb_candidates; j++)
                if (chemins[i][k] > 0 && chemins[k][j] > 0)
                    chemins[i][j] = max(chemins[i][j], min(chemins[i][k], chemins[k][j]));
}

static char *trouverVainqueurSchulze(DataFrame *df, int *idxs_candidats, int nb_candidates, int **chemins, FILE *log, bool debugMode)
{
    char *winner = NULL;
    int best_score = -1;

    for (int i = 0; i < nb_candidates; i++)
    {
        int score = 0;
        for (int j = 0; j < nb_candidates; j++)
        {
            if (i != j && chemins[i][j] > chemins[j][i])
                score++;
        }
        if (score > best_score)
        {
            best_score = score;
            winner = df->columns[idxs_candidats[i]].name;
        }
    }

    return winner;
}

VoteResult voteCondorcetSchulze(DataFrame *df, bool duel, FILE *log, bool debugMode)
{
    int nb_candidates = getNbCandidat(df);
    int idxs_candidats[nb_candidates];
    getIdxsCandidats(df, nb_candidates, idxs_candidats);

    int **chemins = malloc(nb_candidates * sizeof(int *));
    for (int i = 0; i < nb_candidates; i++)
        chemins[i] = malloc(nb_candidates * sizeof(int));

    calculerCheminsFort(df, chemins, idxs_candidats, nb_candidates, log, debugMode);
    char *vainqueur = trouverVainqueurSchulze(df, idxs_candidats, nb_candidates, chemins, log, debugMode);

    for (int i = 0; i < nb_candidates; i++)
        free(chemins[i]);
    free(chemins);

    VoteResult res = createVoteResult(nb_candidates, df->num_rows, 0, vainqueur); // Score peut être 0 ou une autre valeur pertinente
    printResult(res, "cs", 1);

    return res;
}

int main(void)
{
    DataFrame *df = createDataFrameFromCsv("data/calcul1.csv");
    FILE *log = fopen("log", "w");
    VoteResult res = voteCondorcetMinimax(df, true, log, true);
    // VoteResult res2 = voteCondorcetPaires(df, false, log, true);
    // VoteResult res3 = voteCondorcetSchulze(df, false, log, true);
}

#endif
