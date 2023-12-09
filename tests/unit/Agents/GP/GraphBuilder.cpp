/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/[Deprecated]Group7_GP_Agent/GraphBuilder.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Agents/GP/CGPGenotype.hpp"
#include "Agents/GP/GraphBuilder.hpp"
#include "Agents/GP/CGPAgent.hpp"

using namespace cowboys;

CGPAgent mock_agent(0, "mock");

TEST_CASE("Cartesian Graph", "[group7][graph][cartesian]") {
  constexpr size_t INPUT_SIZE = 10;
  constexpr size_t NUM_OUTPUTS = 10;
  constexpr size_t NUM_LAYERS = 2; // Middle layers, does not include input or output layers
  constexpr size_t NUM_NODES_PER_LAYER = 10; // Nodes per middle layer
  constexpr size_t LAYERS_BACK = 2; // How many layers back a node can connect to
  SECTION("Cartesian Graph construction") {
    GraphBuilder builder;

    CGPGenotype genotype({INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
    auto graph = builder.CartesianGraph(genotype, NODE_FUNCTION_SET);

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
    std::vector<double> inputs(INPUT_SIZE, 0);
    for (size_t i = 0; i < NUM_OUTPUTS; ++i)
      actions.at(i) = i;
    GraphBuilder builder;
    bool choose_same_action = true;
    size_t action = 0;

    // Test that the graph is not always choosing the same action when mutated with different seeds
    size_t iterations = 100;
    for (size_t i = 0; i < iterations; ++i) {
      CGPGenotype genotype({INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
      genotype.MutateDefault(1, mock_agent, NODE_FUNCTION_SET.size());
      auto graph = builder.CartesianGraph(genotype, NODE_FUNCTION_SET);
      auto new_action = graph->MakeDecision(inputs, actions);
      choose_same_action = choose_same_action && (new_action == action);
      action = new_action;
    }
    // Could fail, but should be very unlikely
    CHECK_FALSE(choose_same_action);
  }
  SECTION("Cartesian graph mutated header") {
    // Mutating the header should not affect graph behavior to help with mutation locality
    std::vector<size_t> actions(NUM_OUTPUTS, 0);
    std::vector<double> inputs(INPUT_SIZE, 0);
    for (size_t i = 0; i < NUM_OUTPUTS; ++i)
      actions.at(i) = i;
    GraphBuilder builder;
    CGPGenotype base({INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
    auto graph = builder.CartesianGraph(base, NODE_FUNCTION_SET);
    bool choose_same_action = true;
    size_t action = graph->MakeDecision(inputs, actions);

    // The graph should always choose the same action when mutated with different seeds
    size_t iterations = 100;
    for (size_t i = 0; i < iterations; ++i) {
      auto copy = base;
      copy.MutateHeader(1, mock_agent);
      CHECK_FALSE(copy == base);
      auto expanded_graph = builder.CartesianGraph(copy, NODE_FUNCTION_SET);
      auto new_action = expanded_graph->MakeDecision(inputs, actions);
      choose_same_action = choose_same_action && (new_action == action);
      action = new_action;
    }
    // Could fail, but should be very unlikely
    CHECK(choose_same_action);
  }
}