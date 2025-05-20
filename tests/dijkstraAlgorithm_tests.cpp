#include "../directed_graph.h"
#include "../hash_table.hpp"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include <sstream>

// Базовый тест корректности путей
TEST(DijkstraTest, BasicShortestPaths) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    graph.insertNode(3);
    
    graph.addVertex(0, 5.0, 1);
    graph.addVertex(0, 2.0, 2);
    graph.addVertex(2, 1.0, 1);
    graph.addVertex(1, 3.0, 3);

    auto result = graph.dijkstra(0);
    
    EXPECT_DOUBLE_EQ(result[1], 3.0);  // 0->2->1
    EXPECT_DOUBLE_EQ(result[2], 2.0);  // прямое соединение
    EXPECT_DOUBLE_EQ(result[3], 6.0);  // 0->2->1->3
}

// Тест недостижимых узлов
TEST(DijkstraTest, UnreachableNodes) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 1.0, 1);
    graph.addVertex(2, 1.0, 1); // Изолированный узел 2

    auto result = graph.dijkstra(0);
    
    EXPECT_DOUBLE_EQ(result[2], std::numeric_limits<double>::infinity());
}

// Тест графа с одним узлом
TEST(DijkstraTest, SingleNodeGraph) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    
    auto result = graph.dijkstra(0);
    std::stringstream ss;
    result.displayAllItems(ss);
    EXPECT_EQ(ss.str(), "");
}

// Тест обновления путей
TEST(DijkstraTest, PathUpdate) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 5.0, 1);
    graph.addVertex(0, 10.0, 2);
    graph.addVertex(2, 1.0, 1); // Новый короткий путь 0->2->1 (10+1=11 > 5)

    auto result = graph.dijkstra(0);
    EXPECT_DOUBLE_EQ(result[1], 5.0); // Старый путь остается оптимальным
    
    graph.removeVertex(2, 1);
    graph.addVertex(1, 1.0, 2); // Добавляем обратную связь (не влияет)
    result = graph.dijkstra(0);
    EXPECT_DOUBLE_EQ(result[1], 5.0);
}

// Тест отрицательных весов
TEST(DijkstraTest, NegativeWeights) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.addVertex(0, -2.0, 1);
    
    EXPECT_THROW(graph.dijkstra(0), std::logic_error);
}

// Тест несуществующего исходного узла
TEST(DijkstraTest, InvalidOriginNode) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    
    EXPECT_THROW(graph.dijkstra(1), std::invalid_argument);
}

// Тест сложной топологии
TEST(DijkstraTest, ComplexGraph) 
{
    DirectedGraph graph;
    for (size_t i = 0; i < 6; i++) 
    {
        graph.insertNode(i);
    }    
    
    graph.addVertex(0, 7.0, 1);
    graph.addVertex(0, 9.0, 2);
    graph.addVertex(0, 14.0, 5);
    graph.addVertex(1, 10.0, 2);
    graph.addVertex(1, 15.0, 3);
    graph.addVertex(2, 2.0, 5);
    graph.addVertex(2, 11.0, 3);
    graph.addVertex(5, 9.0, 4);
    graph.addVertex(3, 6.0, 4);
    
    auto result = graph.dijkstra(0);
    
    EXPECT_DOUBLE_EQ(result[4], 20.0); // 0->2->5->4 (9+2+9)
    EXPECT_DOUBLE_EQ(result[3], 20.0); // 0->2->3 (9+11)
}

// Тест параллельных рёбер
TEST(DijkstraTest, ParallelEdges) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    
    graph.addVertex(0, 5.0, 1);
    graph.addVertex(0, 3.0, 1); // Более короткое ребро
    
    auto result = graph.dijkstra(0);
    EXPECT_DOUBLE_EQ(result[1], 3.0);
}