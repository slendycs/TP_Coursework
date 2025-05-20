#include "directed_graph.h"
#include <queue>
#include <limits>

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

std::unordered_map<size_t, double> DirectedGraph::dijkstra(size_t origin) const 
{
    if (!adjacencyList_[origin]) throw std::invalid_argument("Origin node does not exist"); // Проверка на существование исходного узла
    if (!isOnlyPositiveVertexes()) throw std::logic_error("This graph contains vertexes with negative weights, which prevents Dijkstra's algorithm from running"); // Проверка что все рёюра положительные

    // Инициализация расстояний
    std::priority_queue<std::pair<double, size_t>, std::vector<std::pair<double, size_t>>, std::greater<>> queue; // Очередь обхода узлов
    std::unordered_map<size_t, double> distances; // таблица узлов и расстояний

    // Установка начальных значений
    for (size_t i = 0; i < adjacencyList_.size(); ++i) 
    {
        if (adjacencyList_[i]) 
        {
            distances[i] = std::numeric_limits<double>::infinity();
        }
    }
    distances[origin] = 0.0;
    queue.emplace(0, origin);

    // Основной цикл обработки узлов
    while (!queue.empty())
    {
        auto [currentDist, currentNode] = queue.top();
        queue.pop();

        if (currentDist > distances[currentNode]) continue;

        if (auto& vertexes = adjacencyList_[currentNode]) 
        {
            // Обход всех смежных узлов
            for (const auto& vertex : *vertexes) 
            {
                // Вычисление нового расстояния
                double newDist = currentDist + vertex.weight_;
                
                // Обновление расстояния, если найден более короткий путь
                if (newDist < distances[vertex.destination_]) 
                {
                    distances[vertex.destination_] = newDist;
                    queue.emplace(newDist, vertex.destination_);
                }
            }
        }
    }

    distances.erase(origin);
    return distances;
}

std::unordered_map<size_t, double> DirectedGraph::bellmanFord(size_t origin) const
{
    // Проверка на существование исходного узла
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node does not exist");

    // Сбор всех рёбер графа
    std::vector<std::tuple<size_t, size_t, double>> allVertexes;
    for (size_t key = 0; key < adjacencyList_.size(); ++key) 
    {
        if (auto& vertexes = adjacencyList_[key])
        {
            for (const auto& vertex : *vertexes)
            {
                allVertexes.emplace_back(key, vertex.destination_, vertex.weight_);
            }
        }
    }

    // Инициализация расстояний
    std::unordered_map<size_t, double> distances; // таблица узлов и расстояний

    // Установка начальных значений
    for (size_t i = 0; i < adjacencyList_.size(); ++i) 
    {
        if (adjacencyList_[i]) 
        {
            distances[i] = std::numeric_limits<double>::infinity();
        }
    }
    distances[origin] = 0.0;

    // Релаксация рёбер (n-1 итераций)
    for (size_t i = 1; i < realSize_; ++i) 
    {
        for (const auto& vertex : allVertexes) 
        {
            size_t start = std::get<0>(vertex);
            size_t destination = std::get<1>(vertex);
            double weight = std::get<2>(vertex);

            if ((distances[start] != std::numeric_limits<double>::infinity()) && (distances[start] + weight < distances[destination])) 
            {
                distances[destination] = distances[start] + weight;
            }
        }
    }

    // Проверка на отрицательные циклы
    for (const auto& vertex : allVertexes) 
    {
        size_t start = std::get<0>(vertex);
        size_t destination = std::get<1>(vertex);
        double weight = std::get<2>(vertex);
        if (distances[start] != std::numeric_limits<double>::infinity() && distances[start] + weight < distances[destination]) 
        {
            throw std::logic_error("Graph contains a negative-weight cycle");
        }
    }

    distances.erase(origin);
    return distances;
}
