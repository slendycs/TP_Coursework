#include "directed_graph.h"

// Приватные методы

DirectedGraph::Vertex *DirectedGraph::searchVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения

    // Получаем указатель на список ребер
    const auto& vertexes = adjacencyList_.at(origin);

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

// Публичные методы

bool DirectedGraph::isEmpty() const
{
    for (const auto& node : adjacencyList_)
    {
        if (node != nullptr) return false;
    }
    return true;
}

bool DirectedGraph::searchNode(size_t key) const
{
    try
    {
        if (adjacencyList_.at(key) != nullptr)
        {
            return true;
        }
        else return false;
    }
    catch(const std::out_of_range)
    {
        return false;
    }
}

void DirectedGraph::insertNode(size_t key)
{
    // Проверяем наличие узла в графе
    if (searchNode(key) == false)
    {
        // Проверяем, можно ли добавить новый узел
        if (key >= size_)
        {
            size_ = key + 1;
            adjacencyList_.resize(size_);
        }

        // Добавляем узел
        adjacencyList_.at(key) = std::make_unique<std::list<Vertex>>();
        realSize_++;
    }
    else throw std::runtime_error("This node already exists in the graph");
}

void DirectedGraph::removeNode(size_t key)
{
    // Проверяем наличие узла
    if (searchNode(key) == false) throw std::invalid_argument("This node is not in the graph");

    // Обходим весь список с узлами
    for (size_t i = 0; i < size_; i++)
    {   
        // Получаем список рёбер
        const auto& nodeVertexes = adjacencyList_.at(i);

        // Удаляем узел
        if (i == key)
        {
            if (nodeVertexes->size() != 0) nodeVertexes->clear();
            adjacencyList_.at(i) = nullptr;
            realSize_--;
        }
        
        // Удаляем ребро от i-того узла до key
        if ((adjacencyList_.at(i) != nullptr) && hasVertex(i, key))
        {
            removeVertex(i, key);
        }
    }
}

void DirectedGraph::addVertex(size_t origin, double weight, size_t destination)
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения
    if (hasVertex(destination, origin) == true) throw std::logic_error("There is already a vertex between these nodes"); // Проверяем не является ли новое ребро обратным

    // Если между двумя нодами уже есть ребро, обновляем вес
    Vertex* temp = searchVertex(origin, destination);
    if (temp != nullptr)
    {
        temp->weight_ = weight;
        return;
    }

    // Если ребро ещё не встречалось, то добавляем его в список рёбер
    auto& originVertexes = adjacencyList_.at(origin);
    originVertexes->push_back(Vertex{weight, destination});
}

bool DirectedGraph::hasVertex(size_t origin, size_t destination) const
{
    return (searchVertex(origin, destination) != nullptr);
}

double DirectedGraph::removeVertex(size_t origin, size_t destination)
{
    // Получаем указатель на список ребер
    auto& originVertexes = adjacencyList_.at(origin);

    // Ищем ребро
    Vertex* temp = searchVertex(origin, destination);
    if (temp == nullptr) throw std::logic_error("Such a vertex does not exist");
    double weight = temp->weight_;

    // Удаляем ребро
    originVertexes->remove(*temp);
    return weight;
}