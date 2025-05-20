#include "../directed_graph.h"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include <limits>

// Тест: базовый граф, волновой алгоритм
TEST(WaveTest, BasicWaveShortestPath) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    graph.insertNode(3);

    graph.addVertex(0, 1.0, 1);
    graph.addVertex(0, 1.0, 2);
    graph.addVertex(2, 1.0, 1);
    graph.addVertex(1, 1.0, 3);

    // Кратчайший путь от 0 до 3 по числу рёбер: 0 -> 1 -> 3 или 0 -> 2 -> 1 -> 3
    EXPECT_EQ(graph.wave(0, 3), 2);
}

// Тест: путь до самого себя
TEST(WaveTest, SelfPath) 
{
    DirectedGraph graph;
    graph.insertNode(0);

    EXPECT_EQ(graph.wave(0, 0), 0);
}

// Тест: отсутствие пути
TEST(WaveTest, NoPathExists) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);

    // Нет рёбер, путь невозможен
    EXPECT_THROW(graph.wave(0, 1), std::logic_error);
}

// Тест: недостижимая вершина
TEST(WaveTest, UnreachableNode) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);

    graph.addVertex(0, 1.0, 1);
    // Узел 2 не соединён

    EXPECT_THROW(graph.wave(0, 2), std::logic_error);
}

// Тест: направление рёбер имеет значение
TEST(WaveTest, DirectedEdgesMatter) 
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);

    graph.addVertex(1, 1.0, 0); // Обратное направление

    // Нет пути из 0 в 1, но есть из 1 в 0
    EXPECT_THROW(graph.wave(0, 1), std::logic_error);
    EXPECT_EQ(graph.wave(1, 0), 1);
}

// Тест: сложная топология с несколькими путями
TEST(WaveTest, MultiplePathsShortestSelected) 
{
    DirectedGraph graph;
    for (size_t i = 0; i < 6; ++i)
        graph.insertNode(i);

    // Два пути от 0 до 4:
    // 0 -> 1 -> 4 (2 ребра)
    // 0 -> 2 -> 3 -> 4 (3 ребра)
    graph.addVertex(0, 1.0, 1);
    graph.addVertex(1, 1.0, 4);
    graph.addVertex(0, 1.0, 2);
    graph.addVertex(2, 1.0, 3);
    graph.addVertex(3, 1.0, 4);

    EXPECT_EQ(graph.wave(0, 4), 2);
}

// Тест: проверка выброса исключения при несуществующих узлах
TEST(WaveTest, InvalidNodes) 
{
    DirectedGraph graph;
    graph.insertNode(0);

    EXPECT_THROW(graph.wave(0, 1), std::invalid_argument);
    EXPECT_THROW(graph.wave(1, 0), std::invalid_argument);
    EXPECT_THROW(graph.wave(5, 5), std::invalid_argument);
}
