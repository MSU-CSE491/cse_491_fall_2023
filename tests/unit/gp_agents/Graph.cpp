/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/Graph.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/Graph.hpp"

using namespace cowboys;

TEST_CASE("Graph", "[group7][graph]") {
    SECTION("Empty Graph") {
        std::vector<size_t> actions{1, 2, 3, 4};
        Graph graph(actions);
        CHECK(graph.GetLayerCount() == 0);
        CHECK(graph.GetNodeCount() == 0);

        // Default output the first action
        std::vector<double> inputs = {1.0, 2.0, 3.0};
        CHECK(graph.MakeDecision(inputs) == actions.at(0));

        std::vector<size_t> actions2{10, 2, 3, 4};
        Graph graph2(actions2);
        CHECK(graph2.MakeDecision(inputs) == actions2.at(0));
    }

    SECTION("Non-Empty Graph") {
        Graph graph({1, 2, 3});
        GraphLayer layer1;
        layer1.push_back(std::make_shared<GraphNode>());
        layer1.push_back(std::make_shared<GraphNode>());
        layer1.push_back(std::make_shared<GraphNode>());
        graph.AddLayer(layer1);
        CHECK(graph.GetLayerCount() == 1);
        CHECK(graph.GetNodeCount() == 3);
    }
}
