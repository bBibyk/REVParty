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

void copyGraph(Graph *dest, Graph *src){
    dest->nb_nodes = src->nb_nodes;
    for (int i = 0; i < dest->nb_nodes; i++){
        strcpy(dest->nodes[i], src->nodes[i]);
    }
    for (int i = 0; i < dest->nb_nodes; i++){
        for (int j = 0; j < dest->nb_nodes; j++){
            dest->matrix[i][j] = src->matrix[i][j];
        }
    }
}

static bool isCycledUtil(Graph *graph, int v, bool visited[], bool *recStack) {
    if (visited[v] == false) {
        visited[v] = true;
        recStack[v] = true;

        for (int i = 0; i < graph->nb_nodes; i++) {
            if (graph->matrix[v][i] != 0) {
                if (!visited[i] && isCycledUtil(graph, i, visited, recStack))
                    return true;
                else if (recStack[i])
                    return true;
            }
        }
    }
    recStack[v] = false;
    return false;
}

bool isCycled(Graph *graph) {
    bool *visited = malloc(graph->nb_nodes * sizeof(bool));
    bool *recStack = malloc(graph->nb_nodes * sizeof(bool));

    for (int i = 0; i < graph->nb_nodes; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    for (int i = 0; i < graph->nb_nodes; i++) {
        if (isCycledUtil(graph, i, visited, recStack))
            return true;
    }

    free(visited);
    free(recStack);
    return false;
}

static void swap(MatrixValue *a, MatrixValue *b)
{
    MatrixValue t = *a;
    *a = *b;
    *b = t;
}

static int partition(MatrixValue *arr, int low, int high)
{
    int pivot = arr[high].value;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].value > pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

static void quickSort(MatrixValue *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void sortedMatrixValues(Graph *graph, int **sortedValues, int **coordinates)
{
    int n = graph->nb_nodes;
    MatrixValue *arr = (MatrixValue *)malloc(n * n * sizeof(MatrixValue));
    *sortedValues = (int *)malloc(n * n * sizeof(int));
    *coordinates = (int *)malloc(n * n * 2 * sizeof(int)); // *2 car on stocke deux coordonnées par valeur

    int index = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[index] = (MatrixValue){graph->matrix[i][j], i, j};
            index++;
        }
    }

    quickSort(arr, 0, n * n - 1);

    for (int i = 0; i < n * n; i++)
    {
        (*sortedValues)[i] = arr[i].value;
        (*coordinates)[i * 2] = arr[i].row;
        (*coordinates)[i * 2 + 1] = arr[i].col;
    }

    free(arr);
}



// Fonction utilitaire pour deleteCycles
// static void deleteCyclesUtil(Graph *graph, int v, bool visited[]) {
//     visited[v] = true;

//     for (int i = 0; i < graph->nb_nodes; i++) {
//         if (graph->matrix[v][i] != 0 && !visited[i]) {
//             deleteCyclesUtil(graph, i, visited);
//             graph->matrix[v][i] = 0;  // Supprimer le cycle
//         }
//     }
// }

// void deleteCycles(Graph *graph) {
//     bool *visited = malloc(graph->nb_nodes * sizeof(bool));

//     for (int i = 0; i < graph->nb_nodes; i++) {
//         visited[i] = false;
//     }

//     for (int i = 0; i < graph->nb_nodes; i++) {
//         if (!visited[i]) {
//             deleteCyclesUtil(graph, i, visited);
//         }
//     }

//     free(visited);
// }


void deleteGraph(Graph *graph){
    free(graph);
}

#endif