#include "graph/graph_io.h"
#include "user_interface/command_handler.cpp"
#include <iostream>


int main()
{
    DirectedGraph graph; // Граф   
    if (readData("../bebra.txt", graph))
    {
        std::cout << "\033[32mData read successfully!\033[0m\n";
        commandHandler(std::cin, std::cout, graph);
    }
    else return 1;
    
    return 0;
}