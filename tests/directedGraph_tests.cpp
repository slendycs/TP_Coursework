#include "../directed_graph.h"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"


TEST(DirectedGraphTest, DefaultConstructor) 
{
    DirectedGraph graph;
    EXPECT_EQ(graph.isEmpty(), true);
    EXPECT_FALSE(graph.searchNode(6)); // Проверка обработки невалидного ключа
}

TEST(DirectedGraphTest, CopyConstructor) 
{
    DirectedGraph original(3);
    original.insertNode(0);
    original.insertNode(1);
    original.addVertex(0, 1.5, 1);
    
    DirectedGraph copy(original);
    EXPECT_TRUE(copy.searchNode(0));
    EXPECT_TRUE(copy.hasVertex(0, 1));
}

TEST(DirectedGraphTest, MoveConstructor) 
{
    DirectedGraph original(3);
    original.insertNode(0);
    original.insertNode(1);
    original.addVertex(0, 1.5, 1);
    
    DirectedGraph moved(std::move(original));
    EXPECT_TRUE(moved.searchNode(0));
    EXPECT_TRUE(moved.hasVertex(0, 1));
    EXPECT_TRUE(original.isEmpty()); // Проверка состояния после перемещения
}

TEST(DirectedGraphTest, CopyAssignment) 
{
    DirectedGraph original(3);
    original.insertNode(0);
    original.insertNode(1);
    original.addVertex(0, 1.5, 1);
    
    DirectedGraph copy = original;
    EXPECT_TRUE(copy.searchNode(0));
    EXPECT_TRUE(copy.hasVertex(0, 1));
}

TEST(DirectedGraphTest, MoveAssignment) 
{
    DirectedGraph original(3);
    original.insertNode(0);
    original.insertNode(1);
    original.addVertex(0, 1.5, 1);
    
    DirectedGraph moved = std::move(original);
    EXPECT_TRUE(moved.searchNode(0));
    EXPECT_TRUE(moved.hasVertex(0, 1));
    EXPECT_TRUE(original.isEmpty());
}

TEST(DirectedGraphTest, InsertAndSearchNode) 
{
    DirectedGraph graph(3);
    graph.insertNode(0);
    graph.insertNode(2);
    
    EXPECT_TRUE(graph.searchNode(0));
    EXPECT_TRUE(graph.searchNode(2));
    EXPECT_FALSE(graph.searchNode(1));
}

TEST(DirectedGraphTest, AddAndHasVertex) 
{
    DirectedGraph graph(3);
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 2.5, 1);
    graph.addVertex(1, 3.0, 2);
    
    EXPECT_TRUE(graph.hasVertex(0, 1));
    EXPECT_TRUE(graph.hasVertex(1, 2));
    EXPECT_FALSE(graph.hasVertex(2, 0));
}

TEST(DirectedGraphTest, RemoveVertex) 
{
    DirectedGraph graph(3);
    graph.insertNode(0);
    graph.insertNode(1);
    graph.addVertex(0, 2.5, 1);
    
    EXPECT_DOUBLE_EQ(graph.removeVertex(0, 1), 2.5);
    EXPECT_FALSE(graph.hasVertex(0, 1));
}

TEST(DirectedGraphTest, RemoveNode) 
{
    DirectedGraph graph(3);
    graph.insertNode(0);
    graph.insertNode(1);
    graph.addVertex(0, 2.5, 1);
    
    graph.removeNode(1);
    EXPECT_FALSE(graph.searchNode(1));
    EXPECT_THROW(graph.hasVertex(0, 1), std::invalid_argument);
}

TEST(DirectedGraphTest, EdgeCases) 
{
    DirectedGraph graph(2);
    // Попытка добавить ребро между несуществующими узлами
    EXPECT_THROW(graph.addVertex(0, 1.0, 1), std::invalid_argument);
    
    graph.insertNode(0);
    // Попытка добавить ребро в несуществующий узел
    EXPECT_THROW(graph.addVertex(0, 1.0, 1), std::invalid_argument);
}

TEST(DirectedGraphTest, Exceptions) 
{
    DirectedGraph graph(3);
    // Удаление несуществующего узла
    EXPECT_THROW(graph.removeNode(0), std::invalid_argument);
    // Проверка несуществующего ребра
    graph.insertNode(0);
    graph.insertNode(1);
    EXPECT_THROW(graph.removeVertex(0, 1), std::logic_error);
}

TEST(DirectedGraphTest, UpdateEdgeWeight) 
{
    DirectedGraph graph(3);
    graph.insertNode(0);
    graph.insertNode(1);
    graph.addVertex(0, 1.0, 1);
    graph.addVertex(0, 2.5, 1); // Обновление веса
    
    EXPECT_DOUBLE_EQ(graph.removeVertex(0, 1), 2.5);
}
