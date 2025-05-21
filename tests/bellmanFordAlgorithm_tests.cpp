#include "../directed_graph.h"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include <sstream>
#include <limits>

// Тест базовых кратчайших путей с отрицательными весами
TEST(BellmanFordTest, BasicShortestPathsWithNegativeWeights) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    graph.insertNode(3);
    
    graph.addVertex(0, -5.0, 1);  // Отрицательный вес
    graph.addVertex(0, 2.0, 2);
    graph.addVertex(2, -1.0, 1);  // Отрицательный вес
    graph.addVertex(1, 3.0, 3);

    auto result = graph.bellmanFord(0);
    
    EXPECT_DOUBLE_EQ(result.at(1), -5.0);  // Прямой путь 0->1
    EXPECT_DOUBLE_EQ(result.at(3), -2.0);  // 0->1->3 (-5 + 3)
}

// Тест недостижимых узлов
TEST(BellmanFordTest, UnreachableNodes) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 1.0, 1);
    graph.addVertex(2, 1.0, 1); // Узел 2 изолирован

    auto result = graph.bellmanFord(0);
    
    EXPECT_TRUE(result.find(2) == result.end() || result.at(2) == std::numeric_limits<double>::infinity());
}

// Тест графа с одним узлом
TEST(BellmanFordTest, SingleNodeGraph) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    
    auto result = graph.bellmanFord(0);
    EXPECT_TRUE(result.empty());
}

// Тест обнаружения отрицательных циклов
TEST(BellmanFordTest, NegativeCycleDetection) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 1.0, 1);
    graph.addVertex(1, -2.0, 2);
    graph.addVertex(2, 1.0, 0); // Цикл 0->1->2->0 (суммарный вес 0)
    EXPECT_NO_THROW(graph.bellmanFord(0));
    
    graph.addVertex(2, -3.0, 0); // Цикл 0->1->2->0 (суммарный вес -4)
    EXPECT_THROW(graph.bellmanFord(0), std::logic_error);
}

// Тест несуществующего исходного узла
TEST(BellmanFordTest, InvalidOriginNode) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    EXPECT_THROW(graph.bellmanFord(1), std::invalid_argument);
}

// Тест сложной топологии с отрицательными рёбрами
TEST(BellmanFordTest, ComplexGraphWithNegativeEdges) 
{
    DirectedGraph graph;
    for (size_t i = 0; i < 5; ++i) {
        graph.insertNode(i);
    }    
    
    graph.addVertex(0, 4.0, 1);
    graph.addVertex(0, 2.0, 2);
    graph.addVertex(1, -1.0, 3);  // Отрицательное ребро
    graph.addVertex(2, 3.0, 1);
    graph.addVertex(3, 2.0, 4);
    graph.addVertex(1, 1.0, 4);
    
    auto result = graph.bellmanFord(0);
    
    EXPECT_DOUBLE_EQ(result.at(4), 5.0);   // 0->1->4 (4+1) или 0->1->3->4 (4-1+2)
    EXPECT_DOUBLE_EQ(result.at(3), 3.0);   // 0->1->3 (4-1)
    EXPECT_DOUBLE_EQ(result.at(1), 4.0);   // Прямое соединение
    EXPECT_DOUBLE_EQ(result.at(2), 2.0);   // Прямое соединение
}

// Тест обновления путей через релаксацию
TEST(BellmanFordTest, PathRelaxation) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    graph.insertNode(3);
    
    graph.addVertex(0, 5.0, 1);
    graph.addVertex(1, 3.0, 2);
    graph.addVertex(2, -4.0, 3);  // Отрицательный вес
    
    auto result = graph.bellmanFord(0);
    EXPECT_DOUBLE_EQ(result.at(3), 4.0);  // 0->1->2->3 (5+3-4)
}

// Тест параллельных рёбер с разными весами
TEST(BellmanFordTest, ParallelEdges) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    
    graph.addVertex(0, 5.0, 1);
    graph.addVertex(0, -3.0, 1); // Более выгодное ребро
    
    auto result = graph.bellmanFord(0);
    EXPECT_DOUBLE_EQ(result.at(1), -3.0);
}

// Тест улучшения пути через отрицательное ребро
TEST(BellmanFordTest, NegativeEdgeImprovement) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 3.0, 1);
    graph.addVertex(1, 4.0, 2);
    graph.addVertex(2, -8.0, 0); // Цикл с отрицательным весом
    
    EXPECT_THROW(graph.bellmanFord(0), std::logic_error);
}