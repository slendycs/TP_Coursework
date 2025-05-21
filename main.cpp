#include "graph_io.h"
#include "directed_graph.h"
#include <iostream>


int main()
{
    DirectedGraph graph; // Граф   
    if (readData("dota.txt", graph))
    {
        std::cout << "Data read!";
    }

    


    return 0;
}