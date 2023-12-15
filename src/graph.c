/**
 * @file graph.c
 * @brief Gestion des graphes adaptés pour la représentation des duels lors des votes Condorcet.
 * @author Bibyk Bogdan
 * @date 07 décembre 2022
 * 
 * Ce fichier contient l'implémentation des fonctions pour l'utilisation des graphes statiques.
 * Les graphes sont orientés et pondérés positivement.
 * 
*/

#ifndef GRAPH_C
#define GRAPH_C

#include "graph.h"

Graph *createGraph(){
    Graph *graph = malloc(sizeof(Graph));
    graph->nb_nodes = 0;
    return graph;
    }

int addNode(Graph *graph, char *node){
    if (graph->nb_nodes >= MAXNODES){
        return -1;
    }
    strcpy(graph->nodes[graph->nb_nodes], node);
    return graph->nb_nodes++;
}

int getNodeIndex(Graph *graph, char *node){
    for (int i = 0; i < graph->nb_nodes; i++){
        if (strcmp(graph->nodes[i], node) == 0){
            return i;
        }
    }
    return -1;
}

void setEdge(Graph *graph, char *from, char *to, int weight){
    if(weight>=0){
        int i = getNodeIndex(graph, from);
        int j = getNodeIndex(graph, to);
        graph->matrix[i][j] = weight;
    }
}

void printGraph(Graph *graph, FILE *file){
    fprintf(file, "   |");
    for (int i = 0; i < graph->nb_nodes; i++){
        fprintf(file, " %c |", ALPHABET[i]);
    }
    fprintf(file, "     Légende :\n");
    for (int i = 0; i < graph->nb_nodes; i++){
        fprintf(file, " %c |", ALPHABET[i]);
        for (int j = 0; j < graph->nb_nodes; j++){
            if(graph->matrix[i][j]==0){
                fprintf(file, " - |");
            } else {
                fprintf(file, "%3d|", graph->matrix[i][j]);
            }
        }
        fprintf(file, "        %c : %s\n", ALPHABET[i], graph->nodes[i]);
    }
}

void removeEdge(Graph *graph, char *from, char *to){
    int i = getNodeIndex(graph, from);
    int j = getNodeIndex(graph, to);
    graph->matrix[i][j] = 0;
}

bool isDominant(Graph *graph, char *node){
    int p = getNodeIndex(graph, node);
    for(int i = 0; i < graph->nb_nodes; i++){
        if(graph->matrix[i][p] != 0){
            return false;
        }
    }
    return true;
}

bool isIsolated(Graph *graph, char *node){
    int p = getNodeIndex(graph, node);
    for(int j = 0; j < graph->nb_nodes; j++){
        if(graph->matrix[p][j] != 0){
            return false;
        }
    }
    return isDominant(graph, node);
}

int removeNode(Graph *graph, char *node){
    int p = getNodeIndex(graph, node);
    for (int j = p; j < graph->nb_nodes - 1; j++){
        strcpy(graph->nodes[j], graph->nodes[j+1]);
    }
    int i, j;
    for (i = 0; i < graph->nb_nodes; i++){
        for (j = p; j < graph->nb_nodes - 1; j++){
            graph->matrix[i][j] = graph->matrix[i][j+1];
        }
    }
    for (j = 0; j < graph->nb_nodes; j++){
        for (i = p; i < graph->nb_nodes - 1; i++){
            graph->matrix[i][j] = graph->matrix[i+1][j];
        }
    }
    return graph->nb_nodes--;
}

void deleteCycles(Graph *graph){
    //TODO
}

void deleteGraph(Graph *graph){
    free(graph);
}

#endif