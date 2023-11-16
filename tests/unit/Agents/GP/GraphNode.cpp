/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/[Deprecated]Group7_GP_Agent/GraphNode.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Agents/GP/GraphNode.hpp"

using namespace cowboys;

TEST_CASE("GraphNode", "[group7][graphnode]") {
  SECTION("Empty GraphNode") {
    GraphNode node;
    CHECK(node.GetOutput() == 0);
    node = GraphNode(7);
    CHECK(node.GetOutput() == 7);
  }
  auto simple_add = [](const GraphNode&node) {
    auto vals = node.GetInputValues<2>({0, 1}).value_or(std::vector<double>{0, 0});
    return vals[0] + vals[1];
  };
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
  std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>(111);
  SECTION("nullptr") {
    CHECK(node->GetOutput() == node->GetDefaultOutput());
    node->SetFunctionPointer(nullptr);
    CHECK(node->GetOutput() == node->GetDefaultOutput());
  }
  SECTION("Sum") {
    node->SetFunctionPointer(Sum);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(1);
    node->AddInput(input);
    CHECK(node->GetOutput() == 1);
    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == 3);
    node->AddInput(std::make_shared<GraphNode>(4));
    CHECK(node->GetOutput() == 7);
    node->AddInput(std::make_shared<GraphNode>(5));
    CHECK(node->GetOutput() == 12);
  }
  SECTION("AnyEq") {
    node->SetFunctionPointer(AnyEq);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
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
    CHECK_NOTHROW(node->GetOutput()); // No inputs
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
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>();
    node->AddInput(input);
    CHECK(node->GetOutput() == 1); // No inputs, but default to true (return 1)

    input->SetDefaultOutput(1); // True -> 0
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(0); // False -> 1
    CHECK(node->GetOutput() == 1);

    input->SetDefaultOutput(10); // True -> 0
    CHECK(node->GetOutput() == 0);
  }
  SECTION("Gate") {
    node->SetFunctionPointer(Gate);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(5);
    node->AddInput(input);
    // One input, default to node default output
    CHECK(node->GetOutput() == node->GetDefaultOutput());

    auto condition = std::make_shared<GraphNode>();
    node->AddInput(condition);
    // condition returns 0 => Condition is false
    CHECK(node->GetOutput() == 0);

    condition->SetDefaultOutput(1);
    // condition returns 1 => Condition is true
    CHECK(node->GetOutput() == 5);

    input->SetDefaultOutput(10);
    CHECK(node->GetOutput() == 10);

    condition->SetDefaultOutput(0);
    CHECK(node->GetOutput() == 0);
  }
  SECTION("Sin") {
    node->SetFunctionPointer(Sin);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == std::sin(1));

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == std::sin(1) + std::sin(2));
  }
  SECTION("Cos") {
    node->SetFunctionPointer(Cos);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 1);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == std::cos(1));

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == std::cos(1) + std::cos(2));
  }
  SECTION("Product") {
    node->SetFunctionPointer(Product);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(2);
    CHECK(node->GetOutput() == 2);

    auto input2 = std::make_shared<GraphNode>(3);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 6);

    input->SetDefaultOutput(4);
    CHECK(node->GetOutput() == 12);
  }
  SECTION("Exp") {
    node->SetFunctionPointer(Exp);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 1);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == std::exp(1));

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == std::exp(1) + std::exp(2));
  }
  SECTION("LessThan") {
    node->SetFunctionPointer(LessThan);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 1); // Default to true

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1); // Default to true

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 1); // 1 < 2 = true

    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == 0); // 3 < 2 = false
  }
  SECTION("GreaterThan") {
    node->SetFunctionPointer(GreaterThan);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 1); // Default to true

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1); // Default to true

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 0); // 1 > 2 = false

    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == 1); // 3 > 2 = true
  }
  SECTION("Max") {
    node->SetFunctionPointer(Max);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1);

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 2);

    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == 3);
  }
  SECTION("Min") {
    node->SetFunctionPointer(Min);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1);

    auto input2 = std::make_shared<GraphNode>(-1);
    node->AddInput(input2);
    CHECK(node->GetOutput() == -1);

    input->SetDefaultOutput(-2);
    CHECK(node->GetOutput() == -2);
  }
  SECTION("NegSum") {
    node->SetFunctionPointer(NegSum);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == -1);

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == -3);

    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == -5);
  }
  SECTION("Square") {
    node->SetFunctionPointer(Square);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1);

    auto input2 = std::make_shared<GraphNode>(2);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 5); // 1^2 + 2^2 = 5

    input->SetDefaultOutput(3);
    CHECK(node->GetOutput() == 13); // 3^2 + 2^2 = 13
  }
  SECTION("PosClamp") {
    node->SetFunctionPointer(PosClamp);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1);

    auto input2 = std::make_shared<GraphNode>(-1);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 1); // pclamp(1) + pclamp(-1) = 1 + 0 = 1

    input->SetDefaultOutput(-2);
    CHECK(node->GetOutput() == 0); // pclamp(-2) + pclamp(-1) = 0 + 0 = 0
  }
  SECTION("NegClamp") {
    node->SetFunctionPointer(NegClamp);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(-1);
    CHECK(node->GetOutput() == -1);

    auto input2 = std::make_shared<GraphNode>(1);
    node->AddInput(input2);
    CHECK(node->GetOutput() == -1); // nclamp(-1) + nclamp(1) = 0 + -1 = -1

    input->SetDefaultOutput(2);
    CHECK(node->GetOutput() == 0); // nclamp(2) + nclamp(1) = 0 + 0 = 0
  }
  SECTION("Sqrt") {
    node->SetFunctionPointer(Sqrt);
    CHECK_NOTHROW(node->GetOutput()); // No inputs
    auto input = std::make_shared<GraphNode>(0);
    node->AddInput(input);
    CHECK(node->GetOutput() == 0);

    input->SetDefaultOutput(1);
    CHECK(node->GetOutput() == 1);

    auto input2 = std::make_shared<GraphNode>(4);
    node->AddInput(input2);
    CHECK(node->GetOutput() == 3); // sqrt(1) + sqrt(4) = 1 + 2 = 3

    input->SetDefaultOutput(9);
    CHECK(node->GetOutput() == 5); // sqrt(9) + sqrt(4) = 3 + 2 = 5

    // Clamp negative values to 0
    input->SetDefaultOutput(-1);
    CHECK(node->GetOutput() == 2); // sqrt(0) + sqrt(4) = 0 + 2 = 2
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

  a->SetDefaultOutput(10);
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
  a->SetDefaultOutput(1);
  // All caches should be invalid
  CHECK(a->IsCacheValid() == false);
  CHECK(b->IsCacheValid() == false);
  CHECK(c->IsCacheValid() == false);
  CHECK(d->IsCacheValid() == false);
}