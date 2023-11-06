/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/GraphNode.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/GraphNode.hpp"

using namespace cowboys;

TEST_CASE("GraphNode", "[group7][graphnode]") {
  SECTION("Empty GraphNode") {
    GraphNode node;
    CHECK(node.GetOutput() == 0);
    node = GraphNode(7);
    CHECK(node.GetOutput() == 7);
  }
  auto simple_add = [](const NodeInputs &inputs) { return inputs.at(0)->GetOutput() + inputs.at(1)->GetOutput(); };
  SECTION("GraphNode function pointers") {
    GraphNode node;
    // Function to sum the first two inputs
    node.SetFunctionPointer(simple_add);

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
    GraphNode node(simple_add);
    auto node1 = std::make_shared<GraphNode>(3);
    auto node2 = std::make_shared<GraphNode>(4);
    node.AddInput(node1);
    node.AddInput(node2);
    CHECK(node.GetOutput() == 7);
  }
}

TEST_CASE("GraphNode function set", "[group7][functionset]") {
  std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();

  SECTION("Sum") {
    node->SetFunctionPointer(Sum);
    node->AddInput(std::make_shared<GraphNode>(3));
    CHECK(node->GetOutput() == 3);
    node->AddInput(std::make_shared<GraphNode>(4));
    CHECK(node->GetOutput() == 7);
    node->AddInput(std::make_shared<GraphNode>(5));
    CHECK(node->GetOutput() == 12);
  }
  SECTION("AnyEq") {
    node->SetFunctionPointer(AnyEq);
    node->AddInput(std::make_shared<GraphNode>(3)); // The value to check for equality
    CHECK(node->GetOutput() == 0);
    node->AddInput(std::make_shared<GraphNode>(4)); // None equal
    CHECK(node->GetOutput() == 0);
    node->AddInput(std::make_shared<GraphNode>(5)); // None equal
    CHECK(node->GetOutput() == 0);
    node->AddInput(std::make_shared<GraphNode>(3)); // One equal
    CHECK(node->GetOutput() == 1);
    node->AddInput(std::make_shared<GraphNode>(4)); // One equal
    CHECK(node->GetOutput() == 1);
  }
  SECTION("And") {
    node->SetFunctionPointer(And);
    node->AddInput(std::make_shared<GraphNode>(1)); // True
    CHECK(node->GetOutput() == 1);
    node->AddInput(std::make_shared<GraphNode>(5)); // True
    CHECK(node->GetOutput() == 1);
    node->AddInput(std::make_shared<GraphNode>(-1)); // True
    CHECK(node->GetOutput() == 1);
    node->AddInput(std::make_shared<GraphNode>(0)); // False
    CHECK(node->GetOutput() == 0);
    node->AddInput(std::make_shared<GraphNode>(1)); // False
    CHECK(node->GetOutput() == 0);
  }
  SECTION("Not") {
    node->SetFunctionPointer(Not);
    auto input = std::make_shared<GraphNode>();
    node->AddInput(input);
    CHECK(node->GetOutput() == 1); // No inputs, but default to true (return 1)

    input->SetOutput(1); // True -> 0
    CHECK(node->GetOutput() == 0);

    input->SetOutput(0); // False -> 1
    CHECK(node->GetOutput() == 1);

    input->SetOutput(10); // True -> 0
    CHECK(node->GetOutput() == 0);
  }
  SECTION("Gate") {
    node->SetFunctionPointer(Gate);
    auto input = std::make_shared<GraphNode>(5);
    node->AddInput(input);
    // One input
    CHECK(node->GetOutput() == 0);

    auto condition = std::make_shared<GraphNode>();
    node->AddInput(condition);
    // condition returns 0 => Condition is false
    CHECK(node->GetOutput() == 0);

    condition->SetOutput(1);
    // condition returns 1 => Condition is true
    CHECK(node->GetOutput() == 5);

    input->SetOutput(10);
    CHECK(node->GetOutput() == 10);

    condition->SetOutput(0);
    CHECK(node->GetOutput() == 0);
  }
}

TEST_CASE("Caching", "[GraphNode]") {
  // Create a graph with 1 input and 2 outputs
  std::shared_ptr<GraphNode> a = std::make_shared<GraphNode>(0);
  std::shared_ptr<GraphNode> b = std::make_shared<GraphNode>(Sum);
  std::shared_ptr<GraphNode> c = std::make_shared<GraphNode>(Sum);
  b->AddInput(a);
  c->AddInput(a);

  // Caches should be invalid
  CHECK(a->IsCacheValid() == false);
  CHECK(b->IsCacheValid() == false);
  CHECK(c->IsCacheValid() == false);

  b->GetOutput();
  // a and b caches should be valid, c should be invalid
  CHECK(a->IsCacheValid() == true);
  CHECK(b->IsCacheValid() == true);
  CHECK(c->IsCacheValid() == false);

  a->SetOutput(10);
  // All caches should be invalid
  CHECK(a->IsCacheValid() == false);
  CHECK(b->IsCacheValid() == false);
  CHECK(c->IsCacheValid() == false);

  c->GetOutput();
  // a and c caches should be valid, b should be invalid
  CHECK(a->IsCacheValid() == true);
  CHECK(b->IsCacheValid() == false);
  CHECK(c->IsCacheValid() == true);

  b->GetOutput();
  // All caches should be valid
  CHECK(a->IsCacheValid() == true);
  CHECK(b->IsCacheValid() == true);
  CHECK(c->IsCacheValid() == true);

  std::shared_ptr<GraphNode> d = std::make_shared<GraphNode>(5);
  d->AddInput(c);
  // All caches except d should be valid
  CHECK(a->IsCacheValid() == true);
  CHECK(b->IsCacheValid() == true);
  CHECK(c->IsCacheValid() == true);
  CHECK(d->IsCacheValid() == false);

  d->GetOutput();
  // All caches should be valid
  CHECK(a->IsCacheValid() == true);
  CHECK(b->IsCacheValid() == true);
  CHECK(c->IsCacheValid() == true);
  CHECK(d->IsCacheValid() == true);

  // Invalidate the cache of a
  a->SetOutput(1);
  // All caches should be invalid
  CHECK(a->IsCacheValid() == false);
  CHECK(b->IsCacheValid() == false);
  CHECK(c->IsCacheValid() == false);
  CHECK(d->IsCacheValid() == false);
}