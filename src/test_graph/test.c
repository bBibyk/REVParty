#include "graph.h"

int main(){
    Graph *graph = createGraph();
    printf("%d\n", addNode(graph, "Booba burger"));
    printf("%d\n", addNode(graph, "1450D UAUAUAU"));
    printf("%d\n", addNode(graph, "Loc"));
    printf("%d\n", addNode(graph, "Pomme-d-tr"));
    setEdge(graph, "Booba burger", "1450D UAUAUAU", 3);
    setEdge(graph, "Booba burger", "Loc", 5);
    setEdge(graph, "1450D UAUAUAU", "Pomme-d-tr", 19);
    setEdge(graph, "Loc", "Pomme-d-tr", 7);
    setEdge(graph, "Pomme-d-tr", "Booba burger", 1);
    printGraph(graph, stdout);
    removeNode(graph, "1450D UAUAUAU");
    printGraph(graph, stdout);
}