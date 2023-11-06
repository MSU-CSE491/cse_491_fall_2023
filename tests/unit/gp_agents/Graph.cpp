/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/Graph.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/CGPGenotype.hpp"
#include "Group7_GP_Agent/Graph.hpp"
#include "Group7_GP_Agent/GraphBuilder.hpp"

using namespace cowboys;

TEST_CASE("Graph", "[group7][graph]") {
  SECTION("Empty Graph") {
    std::vector<size_t> actions{1, 2, 3, 4};
    Graph graph;
    CHECK(graph.GetLayerCount() == 0);
    CHECK(graph.GetNodeCount() == 0);

    // Default output the first action
    std::vector<double> inputs = {1.0, 2.0, 3.0};
    CHECK(graph.MakeDecision(inputs, actions) == actions.at(0));

    std::vector<size_t> actions2{10, 2, 3, 4};
    CHECK(graph.MakeDecision(inputs, actions2) == actions2.at(0));
  }

  SECTION("Non-Empty Graph") {
    Graph graph;
    GraphLayer layer1;
    layer1.push_back(std::make_shared<GraphNode>());
    layer1.push_back(std::make_shared<GraphNode>());
    layer1.push_back(std::make_shared<GraphNode>());
    graph.AddLayer(layer1);
    CHECK(graph.GetLayerCount() == 1);
    CHECK(graph.GetNodeCount() == 3);

    GraphLayer layer2;
    layer2.push_back(std::make_shared<GraphNode>());
    graph.AddLayer(layer2);
    CHECK(graph.GetLayerCount() == 2);
    CHECK(graph.GetNodeCount() == 4);
  }

  SECTION("Mutated Graph") {
    CGPGenotype genotype({8, 4, 2, 10, 2});
    GraphBuilder builder;
    auto graph = builder.CartesianGraph(genotype, FUNCTION_SET);
    auto action = graph->MakeDecision({1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4});

    bool all_same = true;
    for (size_t i = 0; i < 10; ++i) {
      genotype.MutateDefault(1);
      graph = builder.CartesianGraph(genotype, FUNCTION_SET);
      auto new_action = graph->MakeDecision({1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4});
      all_same = all_same && (action == new_action);
    }
    CHECK(!all_same);
  }
}
