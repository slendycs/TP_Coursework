#include "../graph/directed_graph.h"

void help(std::ostream& out)
{
    out << "List of all commands:\n";

    out << "1: \033[32mhelp\033[0m\n";
    out << "   Displays information about all commands on the screen\n";

    out << "2: \033[32mDijkstra\033[0m \033[31m<key>\033[0m\n";
    out << "   Finds the shortest distances from a given node to all nodes using Dijkstra's algorithm\n";

    out << "3: \033[32mBellman-Ford\033[0m \033[31m<key>\033[0m\n";
    out << "   Finds the shortest distances from a given node to all nodes using the Bellman-Ford algorithm\n";

    out << "4: \033[32mWave\033[0m \033[31m<origin>\033[0m \033[31m<destination>\033[0m\n";
    out << "   Finds the shortest distance between nodes using the wave algorithm\n";
}