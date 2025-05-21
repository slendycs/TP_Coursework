#include "../graph/directed_graph.h"
#include "commands.cpp"
#include <iostream>

void commandHandler(std::istream& in, std::ostream& out, DirectedGraph& graph)
{
    std::string commandName;

    out << "Enter command: ";
    while(in >> commandName)
    {
        if (!in) in.clear();

        if (commandName == "help")
        {
            help(out);
        }
        else if (commandName == "Dijkstra")
        {
            
        }
        else if (commandName == "Bellman-Ford")
        {

        }
        else if (commandName == "Wave")
        {

        }
        else
        {
            out << "\033[31mInvalid command!\033[0m\n";
        }
        out << "Enter command: ";
    }
    out << "\n";
}