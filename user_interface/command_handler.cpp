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
            // Считываем аргументы команды
            std::string key;
            in >> key;

            // Проверяем аргумент
            if (isNumber(key))
            {
                dijkstra(std::stoi(key), out, graph);
            }
            else out << "\033[31mInvalid argument!\033[0m\n";
        }
        else if (commandName == "Bellman-Ford")
        {
            // Считываем аргументы команды
            std::string key;
            in >> key;

            // Проверяем аргумент
            if (isNumber(key))
            {
                bellman(std::stoi(key), out, graph);
            }
            else out << "\033[31mInvalid argument!\033[0m\n";
        }
        else if (commandName == "Wave")
        {
            // Считываем аргументы команды
            std::string origin;
            std::string destination;
            in >> origin >> destination;

            if (isNumber(origin) && isNumber(destination))
            {
                out << graph.wave(std::stoi(origin), std::stoi(destination)) << "\n";
            }
            else out << "\033[31mInvalid argument!\033[0m\n";
        }
        else
        {
            out << "\033[31mInvalid command!\033[0m\n";
        }
        out << "Enter command: ";
    }
    out << "\n";
}