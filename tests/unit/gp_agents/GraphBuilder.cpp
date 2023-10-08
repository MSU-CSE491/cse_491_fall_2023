/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/GraphBuilder.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/GraphBuilder.hpp"

using namespace cowboys;

TEST_CASE("Cartesian Graph", "[group7][graph][cartesian]") {
    SECTION("Cartesian Graph construction") {
        constexpr size_t INPUT_SIZE = 10000;
        constexpr size_t NUM_OUTPUTS = 4;
        constexpr size_t NUM_LAYERS = 10;
        constexpr size_t NUM_NODES_PER_LAYER = 10;
        constexpr size_t NUM_NODE_INPUTS = 10;
        constexpr size_t LAYERS_BACK = 2;
        GraphBuilder builder;

        auto graph = builder.CartesianGraph(INPUT_SIZE, NUM_OUTPUTS, NUM_LAYERS, NUM_NODES_PER_LAYER);

        // Input layer + middle layers + output layer
        size_t expected_layer_count = NUM_LAYERS + 2;
        CHECK(graph->GetLayerCount() == expected_layer_count);

        size_t num_input_nodes = INPUT_SIZE;
        size_t num_middle_nodes = NUM_LAYERS * NUM_NODES_PER_LAYER;
        size_t num_output_nodes = NUM_OUTPUTS;
        size_t expected_node_count = num_input_nodes + num_middle_nodes + num_output_nodes;
        CHECK(graph->GetNodeCount() == expected_node_count);
    }
}