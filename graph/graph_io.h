#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include "directed_graph.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

// Структуры для ввода

struct DelimiterIO
{ 
    char exp;
};

struct keyIO
{
    size_t& ref;
};

struct weightIO
{
    double& ref;
};

struct vertexIO
{
    size_t origin;
    double weight;
    size_t destination;
};

// Перегруженные операторы

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in); 
    if (!sentry) return in;

    char c;
    in >> c;
    if (c != dest.exp)
    {
        in.setstate(std::ios::failbit); 
    }
    return in;
}

std::istream& operator>>(std::istream& in, keyIO&& dest)
{
    std::istream::sentry sentry(in); 
    if (!sentry) return in;

    in >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, weightIO&& dest)
{
    std::istream::sentry sentry(in); 
    if (!sentry) return in;

    in >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, vertexIO& dest)
{
    std::istream::sentry sentry(in); 
    if (!sentry) return in;

    return in >> DelimiterIO{'('} >> keyIO{dest.origin} >> DelimiterIO{','} >> weightIO{dest.weight} >> DelimiterIO{','} >> keyIO{dest.destination} >> DelimiterIO{')'};
}

std::istream& operator>>(std::istream& in, DirectedGraph& graph)
{
    std::istream::sentry sentry(in); 
    if (!sentry) return in;

    vertexIO temp;
    if (in >> temp)
    {
        if (graph.searchNode(temp.origin) == false) graph.insertNode(temp.origin);
        if (graph.searchNode(temp.destination) == false) graph.insertNode(temp.destination);
        graph.addVertex(temp.origin, temp.weight, temp.destination);
    }
    return in;
}

// Функция для чтения данных из файла в граф
bool readData(std::string fileName, DirectedGraph& graph)
{   


    // Открываем файл
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "\033[31mError: Failed to open file\033[0m" << "\n";
        return false;
    }

    std::string line; // Строка, считанная из файла

    // Считываем данные из файла
    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> graph;
    }

    // Проверяем правильно ли считался файл
    if (file.bad() || (graph.size() == 0)) 
    {
        std::cerr << "\033[31mWarning: File read error occurred.\033[0m\n";
        file.close();
        return false;
    }

    // Закрываем файл
    file.close();

    return true;
}
#endif