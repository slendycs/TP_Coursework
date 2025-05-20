#include "directed_graph.h"
#include <limits>
#include <queue>

bool DirectedGraph::isEmpty() const
{
    for (const auto& list : adjacencyList_)
    {
        if (list != nullptr) return false;
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

bool DirectedGraph::hasVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро в списке
    for (size_t i = 0; i < originVertexes->getSize(); i++)
    {
        Vertex* temp = originVertexes->at(i);
        if (temp->destination_ == destination) return true;
    }

    return false;
}

double DirectedGraph::removeVertex(size_t origin, size_t destination)
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения

    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро
    Vertex* temp = searchVertex(origin, destination);
    if (temp == nullptr) throw std::logic_error("Such a vertex does not exist");
    double weight = temp->weight_;

    // Удаляем ребро
    originVertexes->deleteItem(*temp);
    return weight;
}

ChainedHashTable<double> DirectedGraph::dijkstra(size_t origin) const
{   
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node does not exist"); // Проверка на существование исходного узла
    if (isOnlyPositiveVertexes() == false) throw std::logic_error("This graph contains vertexes with negative weights, which prevents Dijkstra's algorithm from running"); // Проверка что все рёюра положительные

    // Инициализация расстояний
    ChainedHashTable<double> distances(size_); // таблица узлов и расстояний
    QueueVector<size_t> processingQueue; // Очередь обхода узлов

    // Установка начальных значений
    for (size_t currentNode = 0; currentNode < size_; ++currentNode) 
    {
        if (searchNode(currentNode)) {
            distances.insert(currentNode, std::numeric_limits<double>::infinity());
        }
    }
    distances.insert(origin, 0.0);
    processingQueue.enQueue(origin);

    // Основной цикл обработки узлов
    while (!processingQueue.isEmpty()) 
    {
        size_t currentNode = processingQueue.deQueue();

        // Получение списка рёбер текущего узла
        LinkedList<Vertex>* adjacentVertex = adjacencyList_.at(currentNode);
        if (!adjacentVertex) continue;

        // Обход всех смежных узлов
        size_t vertexesCount = adjacentVertex->getSize();
        for (size_t vertexIndex = 0; vertexIndex < vertexesCount; ++vertexIndex) 
        {
            Vertex* currentVertex = adjacentVertex->at(vertexIndex);
            size_t neighborNode = currentVertex->destination_;
            double vertexWeight = currentVertex->weight_;

            // Вычисление нового расстояния
            double newDistance = distances[currentNode] + vertexWeight;

            // Обновление расстояния, если найден более короткий путь
            if (newDistance < distances[neighborNode]) 
            {
                distances[neighborNode] = newDistance;
                processingQueue.enQueue(neighborNode);
            }
        }
    }
    distances.deleteItem(origin);
    return std::move(distances);
}

ChainedHashTable<double> DirectedGraph::bellmanFord(size_t origin) const
{   
    // Проверка на существование исходного узла
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node does not exist"); 

    // Сбор всех рёбер графа
    std::vector<std::tuple<size_t, size_t, double>> vertexes;
    for (size_t key = 0; key < adjacencyList_.size(); ++key) 
    {
        if (adjacencyList_[key] != nullptr) 
        {
            for (size_t vertexNumber = 0; vertexNumber < adjacencyList_[key]->getSize(); ++vertexNumber) 
            {
                Vertex* vertex = adjacencyList_[key]->at(vertexNumber);
                vertexes.emplace_back(key, vertex->destination_, vertex->weight_);
            }
        }
    }

    // Инициализация расстояний
    ChainedHashTable<double> distances(size_);
    for (size_t node = 0; node < size_; ++node) 
    {
        if (searchNode(node)) 
        {
            distances.insert(node, std::numeric_limits<double>::infinity());
        }
    }
    distances.insert(origin, 0.0);

    // Релаксация рёбер (n-1 итераций)
    for (size_t i = 1; i < realSize_; ++i) 
    {
        for (const auto& vertex : vertexes) 
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
    for (const auto& vertex : vertexes) 
    {
        size_t start = std::get<0>(vertex);
        size_t destination = std::get<1>(vertex);
        double weight = std::get<2>(vertex);
        if (distances[start] != std::numeric_limits<double>::infinity() && distances[start] + weight < distances[destination]) 
        {
            throw std::logic_error("Graph contains a negative-weight cycle");
        }
    }

    distances.deleteItem(origin);
    return distances;
}

size_t DirectedGraph::wave(size_t origin, size_t destination) const
{
    // Проверка на наличие узлов в графе
    if (!searchNode(origin)) throw std::invalid_argument("Origin node is not in the graph");
    if (!searchNode(destination)) throw std::invalid_argument("Destination node is not in the graph");
    if (origin == destination) return 0.0;

    // Очередь для обхода графа в ширину
    QueueVector<size_t> queue;
    // Таблица для отслеживания посещённых узлов
    ChainedHashTable<bool> visited(size_);
    // Таблица расстояний от начального узла до остальных
    ChainedHashTable<size_t> distance(size_);

    // Инициализация начальной вершины
    queue.enQueue(origin);             // Добавляем стартовую вершину в очередь
    visited.insert(origin, true);      // Отмечаем её как посещённую
    distance.insert(origin, 0);        // Расстояние до неё — ноль

    // Основной цикл обхода графа
    while (!queue.isEmpty())
    {
        size_t current = queue.deQueue(); // Извлекаем текущий узел

        // Если достигли целевого узла — возвращаем длину кратчайшего пути
        if (current == destination)
        {
            return distance[current];
        }

        // Получаем список всех соседей текущего узла
        LinkedList<Vertex>* neighbors = adjacencyList_.at(current);
        if (!neighbors) continue;

        // Перебираем всех соседей
        for (size_t i = 0; i < neighbors->getSize(); ++i)
        {
            Vertex* vertex = neighbors->at(i);
            size_t neighbor = vertex->destination_;

            // Если сосед ещё не посещён
            if (!visited[neighbor])
            {
                visited.insert(neighbor, true);                      // Отмечаем его как посещённого
                distance.insert(neighbor, distance[current] + 1);   // Устанавливаем расстояние
                queue.enQueue(neighbor);                            // Добавляем в очередь на обработку
            }
        }
    }

    // Если очередь опустела, а узел не достигнут — пути нет
    throw std::logic_error("No path exists between the nodes");
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
            adjacencyList_.resize(size_, nullptr);
        }

        // Добавляем узел
        adjacencyList_.at(key) = new LinkedList<Vertex>;
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
        LinkedList<Vertex>* nodeVertexes = adjacencyList_.at(i);

        // Удаляем узел
        if (i == key)
        {
            if (nodeVertexes->getSize() != 0) nodeVertexes->clear();
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

DirectedGraph::Vertex *DirectedGraph::searchVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро в списке
    for (size_t i = 0; i < originVertexes->getSize(); i++)
    {
        Vertex* temp = originVertexes->at(i);
        if (temp->destination_ == destination) return temp;
    }

    return nullptr;
}

bool DirectedGraph::isOnlyPositiveVertexes() const
{
    for (const auto& node: adjacencyList_)
    {
        if (node == nullptr) continue;
        for (size_t i = 0; i < node->getSize(); i++)
        {
            Vertex* temp = node->at(i);
            if (temp->weight_ <= 0) return false;
        }
    }
    return true;
}

void DirectedGraph::addVertex(size_t origin, double weight, size_t destination)
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Проверяем не является ли новое ребро обратным
    if (hasVertex(destination, origin) == true) throw std::logic_error("There is already a vertex between these nodes");

    // Если между двумя нодами уже есть ребро, обновляем вес
    Vertex* temp = searchVertex(origin, destination);
    if (temp != nullptr)
    {
        temp->weight_ = weight;
        return;
    }

    // Если ребро ещё не встречалось, то добавляем его в список рёбер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);
    originVertexes->pushBack(Vertex{weight, destination});
}