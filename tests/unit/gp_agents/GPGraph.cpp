/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7GP/GPGraph.hpp"

TEST_CASE("Graph Construction", "[group7][graph]")
{
	SECTION("Empty Graph")
	{
	    std::vector<size_t> actions{1, 2, 3, 4};
		cowboys::Graph graph(actions);
        CHECK(graph.GetLayerCount() == 0);
        CHECK(graph.GetNodeCount() == 0);

        // Default output the first action
        std::vector<double> inputs = {1.0, 2.0, 3.0};
        CHECK(graph.MakeDecision(inputs) == actions.at(0));
        
	    std::vector<size_t> actions2{10, 2, 3, 4};
		cowboys::Graph graph2(actions2);
        CHECK(graph2.MakeDecision(inputs) == actions2.at(0));
        
	}
	/*
	// Test case 1: Test GetLayerCount() with empty graph
    GPGraph graph1({1, 2, 3});
    std::cout << "Layer count: " << graph1.GetLayerCount() << std::endl; // Expected output: 0

    // Test case 2: Test GetNodeCount() with empty graph
    std::cout << "Node count: " << graph1.GetNodeCount() << std::endl; // Expected output: 0

    // Test case 3: Test MakeDecision() with empty graph
    std::vector<double> inputs1 = {1.0, 2.0, 3.0};
    std::cout << "Decision: " << graph1.MakeDecision(inputs1) << std::endl; // Expected output: 1

    // Test case 4: Test GetLayerCount() with non-empty graph
    GPGraph graph2({1, 2, 3});
    graph2.AddNode(std::make_shared<AndNode>());
    graph2.AddNode(std::make_shared<NotNode>());
    graph2.AddNode(std::make_shared<SumNode>());
    std::cout << "Layer count: " << graph2.GetLayerCount() << std::endl; // Expected output: 2

    // Test case 5: Test GetNodeCount() with non-empty graph
    std::cout << "Node count: " << graph2.GetNodeCount() << std::endl; // Expected output: 3

    // Test case 6: Test MakeDecision() with non-empty graph
    std::vector<double> inputs2 = {1.0, 2.0, 3.0};
    std::cout << "Decision: " << graph2.MakeDecision(inputs2) << std::endl; // Expected output: 1
	*/

    SECTION("Non-Empty Graph")
    {
        cowboys::Graph graph({1, 2, 3});
        cowboys::GraphLayer layer1;
        layer1.push_back(std::make_shared<cowboys::AndNode>());
        layer1.push_back(std::make_shared<cowboys::NotNode>());
        layer1.push_back(std::make_shared<cowboys::SumNode>());
        graph.AddLayer(layer1);
        CHECK(graph.GetLayerCount() == 1);
        CHECK(graph.GetNodeCount() == 3);

    }
}
