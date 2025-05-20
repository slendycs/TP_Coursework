#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

class DirectedGraph
{
public:
    // Конструктор по умолчанию
    DirectedGraph():
        size_(5),
        realSize_(0),
        adjacencyList_(size_, nullptr)
    {}

    // Конструктор с параметром
    DirectedGraph(size_t size):
        size_(size),
        realSize_(0),
        adjacencyList_(size_, nullptr)
    {}

    // Конструктор копирования
    DirectedGraph(const DirectedGraph& other): 
        size_(other.size_),
        realSize_(other.realSize_)
    {
        for (const auto& list : other.adjacencyList_) 
        {
            if (list != nullptr)
            {
                adjacencyList_.push_back(new std::list<Vertex>(*list));
            }
            else adjacencyList_.push_back(nullptr);
        }
    }

    // Конструктор перемещения
    DirectedGraph(DirectedGraph&& other) noexcept: 
        size_(other.size_),
        realSize_(other.realSize_),
        adjacencyList_(std::move(other.adjacencyList_)) 
    {
        // Обнуляем исходный объект
        other.size_ = 0;
        other.realSize_ = 0;
        other.adjacencyList_.clear();
    }

    // Оператор копирующего присваивания
    DirectedGraph&operator=(const DirectedGraph &copy)
    {   
        if (this == &copy) return *this;

        // Освобождаем текущие данные
        size_ = copy.size_;
        realSize_ = copy.realSize_;
        for (LinkedList<Vertex>* node : adjacencyList_)
        {
            delete node;
        }
        adjacencyList_.clear();

        // Копируем данные из объекта
        for (const auto& list : copy.adjacencyList_) 
        {
            if (list != nullptr)
            {
                adjacencyList_.push_back(new std::list<Vertex>(*list));
            }
            else adjacencyList_.push_back(nullptr);
        }

        return *this;
    }

    // Оператор перемещающего присваивания
    DirectedGraph& operator=(DirectedGraph&& moved) noexcept 
    {
        if (this == &moved) return *this;
        
        // Освобождаем текущие данные
        for (auto* list : adjacencyList_) 
        {
            delete list;
        }    
        adjacencyList_.clear();
        
        // Переносим данные
        size_ = moved.size_;
        realSize_ = moved.realSize_;
        adjacencyList_ = std::move(moved.adjacencyList_);
        
        // Обнуляем исходник
        moved.size_ = 0;
        moved.realSize_ = 0;
        return *this;
    }

    // Деструктор
    ~DirectedGraph()
    {
        for (LinkedList<Vertex>* node : adjacencyList_)
        {
            delete node;
        }
        adjacencyList_.clear();
    }

    // Методы

    // Проверка наличия узлов в графе
    bool isEmpty() const;

    // Проверка наличия узла в графе
    bool searchNode(size_t key) const;
    // Добавление узла в граф
    void insertNode(size_t key);
    // Удаление узла из графа
    void removeNode(size_t key);

    // Добавление ребра между узлами
    void addVertex(size_t origin, double weight, size_t destination);
    // Проверка наличия ребра между заданными узлами графа
    bool hasVertex(size_t origin, size_t destination) const; 
    // Удаление ребра между заданными узлами графа
    double removeVertex(size_t origin, size_t destination); 

    // Алгоритм Дейкстры для поиска кратчайших путей
    std::unordered_map<double> dijkstra(size_t origin) const;
    // Алгоритм Беллмана — Форда для поиска кратчайших путей
    std::unordered_map<double> bellmanFord(size_t origin) const;
    // Волновой алгоритм для поиска кратчайшего пути между заданной парой вершин
    size_t wave(size_t origin, size_t destination) const;    

private:
    // Структура ребра
    struct Vertex
    {
        double weight_; // Вес ребра
        size_t destination_; // Номер узла назначения

        // Оператор сравнения
        bool operator==(const Vertex& other) const 
        {
            return (weight_ == other.weight_) && (destination_ == other.destination_);
        }
    };

    size_t size_; // Вместимость графа
    size_t realSize_; // Количество узов в графе
    std::vector<std::list<Vertex>*> adjacencyList_; // Представление графа в виде списка смежности

    // Методы

    // Поиск ребра между двумя узлами
    Vertex* searchVertex(size_t origin, size_t destination) const;
    // Проверка имеют ли все рёбра положительные веса
    bool isOnlyPositiveVertexes() const;
};

#endif
