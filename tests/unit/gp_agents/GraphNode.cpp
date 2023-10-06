/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7GP/GraphNode.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7GP/GraphNode.hpp"

using namespace cowboys;

TEST_CASE("GraphNode", "[group7][graphnode]") {
    SECTION("Empty GraphNode") {
        GraphNode node;
        CHECK_THROWS(node.GetInput(0));
        CHECK(node.GetOutput() == 0.0);
    }
    SECTION("Non-Empty GraphNode") {
        GraphNode node;
        auto node1 = std::make_shared<GraphNode>(3);
        auto node2 = std::make_shared<GraphNode>(4);
        node.AddInput(node1);
        node.AddInput(node2);
        CHECK(node.GetInput(0)->GetOutput() == 3);
        CHECK(node.GetInput(1)->GetOutput() == 4);
        CHECK(node.GetOutput() == 0.0);
    }
    SECTION("GraphNode function pointers") {
        GraphNode node;
        // Function to sum the first two inputs
        node.SetFunctionPointer([](const std::vector<double> &inputs) { return inputs.at(0) + inputs.at(1); });

        // Not enough inputs
        CHECK(node.GetOutput() == 0);

        auto node1 = std::make_shared<GraphNode>(3);
        node.AddInput(node1);
        // Still not enough inputs
        CHECK(node.GetOutput() == 0);

        auto node2 = std::make_shared<GraphNode>(4);
        node.AddInput(node2);
        // Now we have enough inputs
        CHECK(node.GetOutput() == 7);
    }
    SECTION("GraphNode function pointer constructor") {
        GraphNode node([](const std::vector<double> &inputs) { return inputs.at(0) + inputs.at(1); });
        auto node1 = std::make_shared<GraphNode>(3);
        auto node2 = std::make_shared<GraphNode>(4);
        node.AddInput(node1);
        node.AddInput(node2);
        CHECK(node.GetOutput() == 7);
    }
}

TEST_CASE("GraphNode function set", "[group7][functionset]") {
    GraphNode node;

    SECTION("Sum") {
        node.SetFunctionPointer(Sum);
        node.AddInput(std::make_shared<GraphNode>(3));
        CHECK(node.GetOutput() == 3);
        node.AddInput(std::make_shared<GraphNode>(4));
        CHECK(node.GetOutput() == 7);
        node.AddInput(std::make_shared<GraphNode>(5));
        CHECK(node.GetOutput() == 12);
    }
    SECTION("AnyEq") {
        node.SetFunctionPointer(AnyEq);
        node.AddInput(std::make_shared<GraphNode>(3)); // The value to check for equality
        CHECK(node.GetOutput() == 0);
        node.AddInput(std::make_shared<GraphNode>(4)); // None equal
        CHECK(node.GetOutput() == 0);
        node.AddInput(std::make_shared<GraphNode>(5)); // None equal
        CHECK(node.GetOutput() == 0);
        node.AddInput(std::make_shared<GraphNode>(3)); // One equal
        CHECK(node.GetOutput() == 1);
        node.AddInput(std::make_shared<GraphNode>(4)); // One equal
        CHECK(node.GetOutput() == 1);
    }
    SECTION("And") {
        node.SetFunctionPointer(And);
        node.AddInput(std::make_shared<GraphNode>(1)); // True
        CHECK(node.GetOutput() == 1);
        node.AddInput(std::make_shared<GraphNode>(5)); // True
        CHECK(node.GetOutput() == 1);
        node.AddInput(std::make_shared<GraphNode>(-1)); // True
        CHECK(node.GetOutput() == 1);
        node.AddInput(std::make_shared<GraphNode>(0)); // False
        CHECK(node.GetOutput() == 0);
        node.AddInput(std::make_shared<GraphNode>(1)); // False
        CHECK(node.GetOutput() == 0);
    }
    SECTION("Not") {
        node.SetFunctionPointer(Not);
        auto input = std::make_shared<GraphNode>();
        node.AddInput(input);
        CHECK(node.GetOutput() == 1); // No inputs, but default to true (return 1)

        input->SetOutput(1); // True -> 0
        CHECK(node.GetOutput() == 0);

        input->SetOutput(0); // False -> 1
        CHECK(node.GetOutput() == 1);

        input->SetOutput(10); // True -> 0
        CHECK(node.GetOutput() == 0);
    }
    SECTION("Gate") {
        node.SetFunctionPointer(Gate);
        auto input = std::make_shared<GraphNode>(5);
        node.AddInput(input);
        // One input
        CHECK(node.GetOutput() == 0);

        auto condition = std::make_shared<GraphNode>();
        node.AddInput(condition);
        // condition returns 0 => Condition is false
        CHECK(node.GetOutput() == 0);

        condition->SetOutput(1);
        // condition returns 1 => Condition is true
        CHECK(node.GetOutput() == 5);

        input->SetOutput(10);
        CHECK(node.GetOutput() == 10);

        condition->SetOutput(0);
        CHECK(node.GetOutput() == 0);
    }
}