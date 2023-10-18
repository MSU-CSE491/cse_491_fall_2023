#pragma once

#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

namespace cowboys {
  class GraphNode;
  using NodeInputs = std::vector<std::shared_ptr<GraphNode>>;
  using NodeFunction = double (*)(const NodeInputs &);

  /// @brief A node in a decision graph.
  /// @note This should always be a shared pointer. Caching will not work otherwise.
  class GraphNode : public std::enable_shared_from_this<GraphNode> {
  protected:
    /// The input nodes to this node.
    std::vector<std::shared_ptr<GraphNode>> inputs;

    /// The function that operates on the outputs from a node's input nodes.
    NodeFunction function_pointer{nullptr};

    /// The default output of this node.
    double output{0};

    /// The nodes connected to this node's output.
    std::vector<std::weak_ptr<GraphNode>> outputs;

    /// The cached output of this node.
    mutable double cached_output{0};

    /// Flag indicating whether the cached output is valid.
    mutable bool cached_output_valid{false};

    void AddOutput(std::weak_ptr<GraphNode> node) { outputs.push_back(node); }

    void RecursiveInvalidateCache() const {
      cached_output_valid = false;
      for (auto &output : outputs) {
        if (auto output_node = output.lock()) {
          output_node->RecursiveInvalidateCache();
        }
      }
    }

  public:
    GraphNode() = default;
    virtual ~GraphNode() = default;

    /// TODO: Check guidelines for this
    GraphNode(double default_value) : output{default_value} {}
    GraphNode(NodeFunction function) : function_pointer{function} {}

    std::string name;

    virtual double GetOutput() const {
      if (cached_output_valid)
        return cached_output;

      double result = output;
      // Invoke function pointer if it exists
      if (function_pointer != nullptr) {
        try {
          result = function_pointer(inputs);
        } catch (const std::out_of_range &e) {
          // Don't do anything, just use the default output
        }
      }

      // Cache the output
      cached_output = result;
      cached_output_valid = true;

      return result;
    }

    void SetFunctionPointer(NodeFunction function) {
      function_pointer = function;
      RecursiveInvalidateCache();
    }

    std::shared_ptr<GraphNode> GetInput(size_t input_idx) const {
      if (inputs.size() == 0) {
        // Throw error if no inputs
        throw std::runtime_error("No inputs");
      }
      if (input_idx >= inputs.size())
        // Return last input if out of bounds
        return inputs.back();
      else
        return inputs.at(input_idx);
    }

    void AddInput(std::shared_ptr<GraphNode> node) {
      inputs.push_back(node);
      // Add a weak pointer to this node to the input node's outputs
      node->AddOutput(weak_from_this());
      RecursiveInvalidateCache();
    }
    void AddInputs(const std::vector<std::shared_ptr<GraphNode>> &nodes) {
      inputs.insert(inputs.end(), nodes.begin(), nodes.end());
      RecursiveInvalidateCache();
    }
    void SetInputs(const std::vector<std::shared_ptr<GraphNode>> &nodes) {
      inputs = nodes;
      RecursiveInvalidateCache();
    }
    void SetOutput(double value) {
      if (output != value) {
        output = value;
        RecursiveInvalidateCache();
      }
    }
    bool IsCacheValid() const { return cached_output_valid; }
  };

  /// @brief Sum all inputs.
  /// @param inputs Vector of doubles representing the inputs.
  /// @return The sum of all inputs.
  double Sum(const NodeInputs &inputs) {
    return std::transform_reduce(inputs.cbegin(), inputs.cend(), 0, std::plus{},
                                 [](const auto &node) { return node->GetOutput(); });
  }

  /// @brief Check if all inputs are not equal to 1.
  /// @param inputs Vector of doubles representing the inputs.
  /// @return 1 if all inputs are not equal to 0, 0 otherwise.
  double And(const NodeInputs &inputs) {
    for (const auto &input : inputs) {
      if (input->GetOutput() == 0.)
        return 0.;
    }
    return 1.;
  }

  /// @brief Check if any of the inputs besides the first are equal to the
  /// first input.
  /// @param inputs Vector of doubles representing the inputs.
  /// @return 1 if any of the inputs besides the first are equal to the first
  /// input, 0 otherwise.
  double AnyEq(const NodeInputs &inputs) {
    for (size_t i = 1; i < inputs.size(); ++i) {
      if (inputs.at(0)->GetOutput() == inputs.at(i)->GetOutput())
        return 1.;
    }
    return 0.;
  }

  /// @brief Check if the first input is equal to 0.
  /// @param inputs Vector of doubles representing the inputs.
  /// @return 1 if the first input is equal to 0 or there are no inputs, 0
  /// otherwise.
  double Not(const NodeInputs &inputs) { return (inputs.size() == 0) || (inputs.at(0)->GetOutput() == 0.) ? 1. : 0.; }

  /// @brief Returns the input with index 0 if the condition (input with index
  /// 1) is not 0.
  /// @param inputs Vector of doubles representing the inputs.
  /// @return The value of the input with index 0 if the condition (input with
  /// index 1) is not 0, 0 otherwise.
  double Gate(const NodeInputs &inputs) { return inputs.at(1)->GetOutput() != 0. ? inputs.at(0)->GetOutput() : 0.; }

  /// @brief A vector of all the node functions.
  const std::vector<NodeFunction> FUNCTION_SET{Sum, And, AnyEq, Not, Gate};
} // namespace cowboys