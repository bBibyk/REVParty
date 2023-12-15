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

typedef struct Graph {
    int matrix[MAXNODES][MAXNODES];
    char nodes[MAXNODES][MAXCHAR];
    int nb_nodes;
} Graph;

Graph *createGraph();

int addNode(Graph *graph, char *node);

int getNodeIndex(Graph *graph, char *node);

void setEdge(Graph *graph, char *from, char *to, int weight);

void printGraph(Graph *graph, FILE *file);

void removeEdge(Graph *graph, char *from, char *to);

bool isDominant(Graph *graph, char *node);

bool isIsolated(Graph *graph, char *node);

int removeNode(Graph *graph, char *node);

void deleteCycles(Graph *graph);

void deleteGraph(Graph *graph);

#endif