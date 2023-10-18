/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/GraphBuilder.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/CGPGenotype.hpp"
#include "Group7_GP_Agent/GraphBuilder.hpp"

using namespace cowboys;

TEST_CASE("Cartesian Graph", "[group7][graph][cartesian]") {
  constexpr size_t INPUT_SIZE = 10;
  constexpr size_t NUM_OUTPUTS = 10;
  constexpr size_t NUM_LAYERS = 2;
  constexpr size_t NUM_NODES_PER_LAYER = 10;
  constexpr size_t NUM_NODE_INPUTS = 10;
  constexpr size_t LAYERS_BACK = 2;
  SECTION("Cartesian Graph construction") {
    GraphBuilder builder;

    CGPGenotype genotype({INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
    auto graph = builder.CartesianGraph(genotype, FUNCTION_SET);

    // Input layer + middle layers + output layer
    size_t expected_layer_count = NUM_LAYERS + 2;
    CHECK(graph->GetLayerCount() == expected_layer_count);

    size_t num_input_nodes = INPUT_SIZE;
    size_t num_middle_nodes = NUM_LAYERS * NUM_NODES_PER_LAYER;
    size_t num_output_nodes = NUM_OUTPUTS;
    size_t expected_node_count = num_input_nodes + num_middle_nodes + num_output_nodes;
    CHECK(graph->GetNodeCount() == expected_node_count);
  }
  SECTION("Cartesian graph mutated") {
    std::vector<size_t> actions(NUM_OUTPUTS, 0);
    for (size_t i = 0; i < NUM_OUTPUTS; ++i)
      actions.at(i) = i;
    GraphBuilder builder;
    size_t iterations = 100;
    bool choose_first_action_only = true;
    for (size_t i = 0; i < iterations; ++i) {
      CGPGenotype genotype({INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
      genotype.SetSeed(i);
      genotype.MutateConnections(0.5);
      genotype.MutateFunctions(0.5, FUNCTION_SET.size());
      auto graph = builder.CartesianGraph(genotype, FUNCTION_SET);
      auto action_to_take = graph->MakeDecision(std::vector<double>(INPUT_SIZE, 1.0), actions);
      choose_first_action_only = choose_first_action_only && action_to_take == actions.at(0);
    }
    // Can fail when using random seeds, but should be very unlikely
    CHECK(!choose_first_action_only);
  }
}