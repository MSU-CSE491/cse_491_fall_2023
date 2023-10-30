#pragma once

#include "CGPGenotype.hpp"
#include "Graph.hpp"

namespace cowboys {

  /// @brief A class for building graphs.
  class GraphBuilder {
  public:
    GraphBuilder() = default;
    ~GraphBuilder() = default;

    /// @brief Creates a decision graph from a CGP genotype.
    /// @param genotype The genotype to create the decision graph from.
    /// @return The decision graph.
    std::unique_ptr<Graph> CartesianGraph(const CGPGenotype &genotype, const std::vector<NodeFunction> &function_set) {
      auto decision_graph = std::make_unique<Graph>();

      //
      // Add all the nodes
      //
      // Input layer
      GraphLayer input_layer;
      for (size_t i = 0; i < genotype.GetNumInputs(); ++i) {
        input_layer.emplace_back(std::make_shared<GraphNode>(0));
      }
      decision_graph->AddLayer(input_layer);

      // Middle Layers
      for (size_t i = 0; i < genotype.GetNumLayers(); ++i) {
        GraphLayer layer;
        for (size_t j = 0; j < genotype.GetNumNodesPerLayer(); ++j) {
          layer.emplace_back(std::make_shared<GraphNode>(0));
        }
        decision_graph->AddLayer(layer);
      }

      // Action layer
      GraphLayer output_layer;
      for (size_t i = 0; i < genotype.GetNumOutputs(); ++i) {
        output_layer.emplace_back(std::make_shared<GraphNode>(0));
      }
      decision_graph->AddLayer(output_layer);

      //
      // Configure graph based on genotype
      //

      auto functional_nodes = decision_graph->GetFunctionalNodes();
      auto all_nodes = decision_graph->GetNodes();
      auto nodes_it = functional_nodes.cbegin();
      auto genes_it = genotype.cbegin();
      // Iterator distances should be the same
      assert(std::distance(functional_nodes.cend(), functional_nodes.cbegin()) ==
             std::distance(genotype.cend(), genotype.cbegin()));
      // Get the iterator of all nodes and move it to the start of the first functional node
      auto all_nodes_it = all_nodes.cbegin() + genotype.GetNumInputs();
      for (; nodes_it != functional_nodes.end() && genes_it != genotype.cend(); ++nodes_it, ++genes_it) {
        // Advance the all nodes iterator if we are at the start of a new layer
        auto dist = std::distance(functional_nodes.cbegin(), nodes_it);
        if (dist != 0 && dist % genotype.GetNumNodesPerLayer() == 0) {
          std::advance(all_nodes_it, genotype.GetNumNodesPerLayer());
        }

        auto &[connections, function_idx, output] = *genes_it;
        (*nodes_it)->SetFunctionPointer(function_set.at(function_idx));
        (*nodes_it)->SetOutput(output);
        
        // Copy the all nodes iterator and move it backwards by the number of connections
        auto nodes_it_copy = all_nodes_it;
        std::advance(nodes_it_copy, -connections.size());
        // Add the inputs to the node
        for (auto &connection : connections) {
          if (connection != '0') {
            (*nodes_it)->AddInput(*nodes_it_copy);
          }
          ++nodes_it_copy;
        }
      }

      return decision_graph;
    }

    /// @brief Creates a decision graph for pacing up and down in a
    /// MazeWorld. Assumes that the inputs are in the format: prev_action,
    /// current_state, above_state, below_state, left_state, right_state
    /// @param action_vec Assumes that the action outputs are in the format:
    /// up, down, left, right
    /// @return The decision graph for a vertical pacer.
    std::unique_ptr<Graph> VerticalPacer() {
      auto decision_graph = std::make_unique<Graph>();

      GraphLayer input_layer;
      std::shared_ptr<GraphNode> prev_action = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> current_state = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> above_state = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> below_state = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> left_state = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> right_state = std::make_shared<GraphNode>(0);
      input_layer.insert(input_layer.end(),
                         {prev_action, current_state, above_state, below_state, left_state, right_state});
      decision_graph->AddLayer(input_layer);

      // state == 1 => floor which is walkable
      GraphLayer obstruction_layer;
      std::shared_ptr<GraphNode> up_not_blocked = std::make_shared<GraphNode>(AnyEq);
      up_not_blocked->AddInputs(GraphLayer{above_state, std::make_shared<GraphNode>(1)});
      std::shared_ptr<GraphNode> down_not_blocked = std::make_shared<GraphNode>(AnyEq);
      down_not_blocked->AddInputs(GraphLayer{below_state, std::make_shared<GraphNode>(1)});
      obstruction_layer.insert(obstruction_layer.end(), {up_not_blocked, down_not_blocked});
      decision_graph->AddLayer(obstruction_layer);

      // Separate previous action into up and down nodes
      GraphLayer prev_action_layer;
      std::shared_ptr<GraphNode> up_prev_action = std::make_shared<GraphNode>(AnyEq);
      up_prev_action->AddInputs(GraphLayer{prev_action, std::make_shared<GraphNode>(1)});
      std::shared_ptr<GraphNode> down_prev_action = std::make_shared<GraphNode>(AnyEq);
      down_prev_action->AddInputs(GraphLayer{prev_action, std::make_shared<GraphNode>(2)});
      prev_action_layer.insert(prev_action_layer.end(), {up_prev_action, down_prev_action});
      decision_graph->AddLayer(prev_action_layer);

      GraphLayer moving_layer;
      // If up_not_blocked and up_prev_action ? return 1 : return 0
      // If down_not_blocked and down_prev_action ? return 1 : return 0
      std::shared_ptr<GraphNode> keep_up = std::make_shared<GraphNode>(And);
      keep_up->AddInputs(GraphLayer{up_not_blocked, up_prev_action});
      std::shared_ptr<GraphNode> keep_down = std::make_shared<GraphNode>(And);
      keep_down->AddInputs(GraphLayer{down_not_blocked, down_prev_action});
      moving_layer.insert(moving_layer.end(), {keep_up, keep_down});
      decision_graph->AddLayer(moving_layer);

      // If down_blocked, turn_up
      // If up_blocked, turn_down
      GraphLayer turn_layer;
      std::shared_ptr<GraphNode> turn_up = std::make_shared<GraphNode>(Not);
      turn_up->AddInputs(GraphLayer{down_not_blocked});
      std::shared_ptr<GraphNode> turn_down = std::make_shared<GraphNode>(Not);
      turn_down->AddInputs(GraphLayer{up_not_blocked});
      turn_layer.insert(turn_layer.end(), {turn_up, turn_down});
      decision_graph->AddLayer(turn_layer);

      // Output layer, up, down, left, right
      GraphLayer action_layer;
      // move up = keep_up + turn_up,
      // move down = keep_down + turn_down,
      std::shared_ptr<GraphNode> up = std::make_shared<GraphNode>(Sum);
      up->AddInputs(GraphLayer{keep_up, turn_up});
      std::shared_ptr<GraphNode> down = std::make_shared<GraphNode>(Sum);
      down->AddInputs(GraphLayer{keep_down, turn_down});
      std::shared_ptr<GraphNode> left = std::make_shared<GraphNode>(0);
      std::shared_ptr<GraphNode> right = std::make_shared<GraphNode>(0);
      action_layer.insert(action_layer.end(), {up, down, left, right});
      decision_graph->AddLayer(action_layer);

      return decision_graph;
    }
  };
} // namespace cowboys