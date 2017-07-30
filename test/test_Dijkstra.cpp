#include "gtest/gtest.h"
#include "tastylib/Dijkstra.h"
#include <vector>
#include <string>

using namespace tastylib;
using std::vector;
using std::string;

typedef DijkGraph<string> G;
typedef G::NumType NumType;
typedef G::WeightType WeightType;

TEST(Dijkstra, Simple) {
    G graph(3, G::LIST);
    graph[0].val = "Alice";
    graph[1].val = "Darth";
    graph[2].val = "Bob";
    graph.setWeight(0, 1, 5);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 5);
    dijkstra(graph, 0);
    vector<WeightType> expectDist = {0, 5, 10};
    for (NumType i = 0; i < graph.getSize(); ++i) {
        EXPECT_EQ(graph[i].dist, expectDist[i]);
    }
    vector<NumType> expectPrev = {4, 0, 1};
    for (NumType i = 0; i < graph.getSize(); ++i) {
        EXPECT_EQ(graph[i].prev, expectPrev[i]);
    }
    EXPECT_TRUE(graph[0].val == "Alice");
    EXPECT_TRUE(graph[1].val == "Darth");
    EXPECT_TRUE(graph[2].val == "Bob");
}

TEST(Dijkstra, Complex) {
    G graph(8, G::MATRIX);
    graph.setWeight(1, 0, 1);
    graph.setWeight(1, 2, 2);
    graph.setWeight(1, 4, 1);
    graph.setWeight(2, 4, 3);
    graph.setWeight(2, 5, 10);
    graph.setWeight(3, 1, 4);
    graph.setWeight(3, 6, 5);
    graph.setWeight(4, 3, 2);
    graph.setWeight(4, 5, 2);
    graph.setWeight(4, 6, 8);
    graph.setWeight(4, 7, 4);
    graph.setWeight(5, 7, 6);
    graph.setWeight(7, 6, 1);

    {
        dijkstra(graph, 1);
        vector<WeightType> expectDist = {1, 0, 2, 3, 1, 3, 6, 5};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            EXPECT_EQ(graph[i].dist, expectDist[i]);
        }
        vector<NumType> expectPrev = {1, 9, 1, 4, 1, 4, 7, 4};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            EXPECT_EQ(graph[i].prev, expectPrev[i]);
        }
    }

    {
        dijkstra(graph, 2);
        vector<WeightType> expectDist = {10, 9, 0, 5, 3, 5, 8, 7};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            EXPECT_EQ(graph[i].dist, expectDist[i]);
        }
        vector<NumType> expectPrev = {1, 3, 9, 4, 2, 4, 7, 4};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            EXPECT_EQ(graph[i].prev, expectPrev[i]);
        }
    }
}
