#include <iostream>

#include "graph.h"

int main(int argc, char** argv) {
    Graph<std::size_t, double> graph;
    graph.insert_vertex(0);
    graph.insert_vertex(1);
    graph.insert_vertex(2);
    graph.insert_edge(4.0, 0, 1);
    graph.insert_edge(8.0, 0, 2);

    auto edge1 = graph.are_adjacent(0, 1);
    auto edge2 = graph.are_adjacent(0, 2);
    auto edge3 = graph.are_adjacent(1, 2);

    if (edge1) std::cout << "Edge 1 has value " << edge1.value() << "\n";
    if (edge2) std::cout << "Edge 2 has value " << edge2.value() << "\n";
    if (edge3) std::cout << "Edge 3 has value " << edge3.value() << "\n";

    return 0;
}
