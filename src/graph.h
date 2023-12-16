/**
 * @file graph.h
 * @brief Définitionon des graphes adaptés pour la représentation des duels lors des votes Condorcet.
 * @author Bibyk Bogdan
 * @date 07 décembre 2022
 *
 * Ce fichier d'en-tete contient la structure des graphes statiques
 * et définit des fonctions pour les manipuler.
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXCHAR 1024
#define MAXNODES 10 // Eviter d'allet au-delà de 26 pour l'alphabet qui sert à l'affichage
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef struct Graph
{
    int matrix[MAXNODES][MAXNODES];
    char nodes[MAXNODES][MAXCHAR];
    int nb_nodes;
} Graph;

typedef struct
{
    int value;
    int row;
    int col;
} MatrixValue;

/**
 * @fn Graph *createGraph()
 * @brief Fonction d'initialisation d'un graphe.
 * @return Structure représentat le graphe vide.
 */
Graph *createGraph();

/**
 * @fn int addNode(Graph *graph, char *node)
 * @brief Ajout d'un noe dans le graphe.
 * @param[in] graph Structure graph à modifier.
 * @param[in] node Nom du noeud à ajouter.
 * @return L'indice du nom du noeud.
 */
int addNode(Graph *graph, char *node);

/**
 * @fn int getNodeIndex(Graph *graph, char *node)
 * @brief Retourne l'indice d'un noeud dans le graphe.
 * @param[in] graph Structure graph.
 * @param[in] node Nom du noeud à trouver.
 */
int getNodeIndex(Graph *graph, char *node);

/**
 * @fn void setEdge(Graph *graph, char *from, char *to, int weight)
 * @brief Ajout d'un lien entre deux noeuds.
 * @param[in] graph Structure graph à modifier.
 * @param[in] from Nom du noeud de depart.
 * @param[in] to Nom du noeud d'arrivé.
 * @param[in] weight Poids du lien.
 */
void setEdge(Graph *graph, char *from, char *to, int weight);

/**
 * @fn void printGraph(Graph *graph, FILE *file)
 * @brief Affichage du graphe.
 * @param[in] graph Structure graph à afficher.
 * @param[out] file Fichier de sortie.
 */
void printGraph(Graph *graph, FILE *file);

/**
 * @fn void removeEdge(Graph *graph, char *from, char *to)
 * @brief Suppression d'un lien entre deux noeuds.
 * @param[in] graph Structure graph à modifier.
 * @param[in] from Nom du noeud de depart.
 * @param[in] to Nom du noeud d'arrivé.
 */
void removeEdge(Graph *graph, char *from, char *to);

/**
 * @fn bool isDominant(Graph *graph, char *node)
 * @brief Indique si un noeud est dominant.
 * @param[in] graph Structure graph.
 * @param[in] node Nom du noeud à tester.
 * @return Vrai si le noeud est dominant.
 */
bool isDominant(Graph *graph, char *node);

/**
 * @fn bool isIsolated(Graph *graph, char *node)
 * @brief Indique si un noeud est isole.
 * @param[in] graph Structure graph.
 * @param[in] node Nom du noeud à tester.
 * @return Vrai si le noeud est isole.
 */
bool isIsolated(Graph *graph, char *node);

/**
 * @fn int removeNode(Graph *graph, char *node)
 * @brief Suppression d'un noeud.
 * @param[in] graph Structure graph à modifier.
 * @param[in] node Nom du noeud à supprimer.
 * @return L'indice du noeud supprimé.
 */
int removeNode(Graph *graph, char *node);

void copyGraph(Graph *dest, Graph *src);

/**
 * @fn void deleteCycles(Graph *graph)
 * @brief Suppression des liens cycliques.
 * @param[in] graph Structure graph à vérifier.
 * @return Vrai si le graphe est cyclique.
 */
bool isCycled(Graph *graph);

/**
 * @fn void deleteGraph(Graph *graph)
 * @brief Suppression du graphe.
 * @param[in] graph Structure graph à supprimer.
 */
void deleteGraph(Graph *graph);

/**
 * @fn void sortedMatrixValues(Graph *graph, int **sortedValues, int **coordinates)
 * @brief Tri des valeurs de la matrice du graphe.
 * @param[in] graph Structure graph à trier.
 * @param[out] sortedValues Tableau des valeurs triées.
 * @param[out] coordinates Tableau des coordonnées des valeurs triées.
 */
void sortedMatrixValues(Graph *graph, int **sortedValues, int **coordinates);

#endif