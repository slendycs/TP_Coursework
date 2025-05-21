#include "graph/graph_io.h"
#include "graph/directed_graph.h"
#include <iostream>


int main()
{
    DirectedGraph graph; // Граф   
    if (readData("../bebra.txt", graph))
    {
        std::cout << "Data read!";
    }

    


    return 0;
}