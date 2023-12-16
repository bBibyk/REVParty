#include "graph.h"

int main(){
    Graph *graph = createGraph();
    printf("%d\n", addNode(graph, "A"));
    printf("%d\n", addNode(graph, "B"));
    printf("%d\n", addNode(graph, "C"));
    printf("%d\n", addNode(graph, "D"));
    setEdge(graph, "D", "B", 1);
    setEdge(graph, "C", "D", 2);
    setEdge(graph, "B", "C", 3);
    printGraph(graph, stdout);
    printf("%d\n", isCycled(graph));
}