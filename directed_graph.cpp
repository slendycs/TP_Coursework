#include "directed_graph.h"

// Приватные методы

DirectedGraph::Vertex *DirectedGraph::searchVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения

    // Получаем указатель на список ребер
    const auto& vertexes = adjacencyList_[origin];

    // Ищем ребро в списке
    for (auto& vertex : *vertexes) 
    {
        if (vertex.destination_ == destination) 
        {
            // Возвращаем указатель на найденную вершину
            return &vertex;
        }
    }

    return nullptr;
}

bool DirectedGraph::isOnlyPositiveVertexes() const
{
    // Перебираем каждый узел в графе
    for (const auto& node: adjacencyList_)
    {
        if (node == nullptr) continue; // Если узел не существует, переходим к следующему

         for (const auto& vertex : *node.get()) 
        {
           if (vertex.weight_ <= 0) return false;
        }
    }
    return true;
}
