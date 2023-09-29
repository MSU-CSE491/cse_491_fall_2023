
#pragma once

#include <cassert>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

namespace cowboys
{
    class GraphNode
    {
    protected:
        std::vector<std::shared_ptr<GraphNode>> inputs;
        std::vector<size_t> inputs_indices;

        /// The output of this node.
        double output = 0.;

    public:
        GraphNode() = default;
        virtual ~GraphNode() = default;

        /// TODO: Check guidelines for this
        GraphNode(double default_value) : output{default_value} {}

        /// @brief Constructor for input indices
        /// @param input_indices
        GraphNode(std::vector<size_t> input_indices) : inputs_indices{input_indices} {}

        std::string name;

        /// TODO: Cache outputs
        virtual double GetOutput() const { return output; }

        std::shared_ptr<GraphNode> GetInput(size_t input_idx) const
        {
            if (input_idx >= inputs.size())
                // Return last input if out of bounds
                return inputs.back();
            else
                return inputs.at(input_idx);
        }

        void AddInput(std::shared_ptr<GraphNode> node) { inputs.push_back(node); }
        void AddInputs(const std::vector<std::shared_ptr<GraphNode>> &nodes) { inputs.insert(inputs.end(), nodes.begin(), nodes.end()); }
        void SetInputs(const std::vector<std::shared_ptr<GraphNode>> &nodes) { inputs = nodes; }
        void SetOutput(double value) { output = value; }
    };

    class GateNode : public GraphNode
    {
    public:
        GateNode() = default;
        virtual ~GateNode() = default;

        double GetOutput() const override
        {
            // input 0 is the input
            // input 1 is the condition
            if (inputs.size() < 2)
                return 0.;

            // Let the signal through if the condition is not 0
            return GetInput(1)->GetOutput() != 0. ? GetInput(0)->GetOutput() : 0.;
        }
    };

    class AndNode : public GraphNode
    {
    public:
        AndNode() = default;
        virtual ~AndNode() = default;

        double GetOutput() const override
        {
            // input 0 and 1 are the inputs
            if (inputs.size() < 2)
                return 0.;
            auto condition = GetInput(0)->GetOutput() != 0. && GetInput(1)->GetOutput() != 0.;
            return condition ? 1. : 0.;
        }
    };

    class AnyEqNode : public GraphNode
    {
    public:
        AnyEqNode() = default;
        virtual ~AnyEqNode() = default;

        double GetOutput() const override
        {
            // input 0 is the input, the rest is the values that we are checking against
            if (inputs.size() < 2)
                return 0.;
            for (size_t i = 1; i < inputs.size(); ++i)
            {
                auto condition = GetInput(0)->GetOutput() == GetInput(i)->GetOutput();
                if (condition)
                    return 1.;
            }
            return 0.;
        }
    };

    class NotNode : public GraphNode
    {
    public:
        NotNode() = default;
        virtual ~NotNode() = default;

        double GetOutput() const override
        {
            // input 0 is the value to not
            if (inputs.size() < 1)
                return 0.;
            auto condition = GetInput(0)->GetOutput() == 0.;
            return condition ? 1. : 0.;
        }
    };

    class SumNode : public GraphNode
    {
    public:
        SumNode() = default;
        virtual ~SumNode() = default;

        double GetOutput() const override
        {
            // Sum all inputs
            double sum = 0.;
            for (auto &input : inputs)
            {
                sum += input->GetOutput();
            }
            return sum;
        }
    };

    class Graph
    {
    protected:
        /// Actions that can be taken.
        std::vector<size_t> actions;

        /// Layers of nodes in the graph.
        std::vector<std::vector<std::shared_ptr<GraphNode>>> layers;

    public:
        Graph(const std::vector<size_t> &action_vec) : actions{action_vec} { assert(actions.size() > 0); }
        ~Graph() = default;

        size_t MakeDecision(const std::vector<double> &inputs)
        {
            if (layers.size() == 0)
                return actions.at(0);

            // Set inputs of input layer
            size_t i = 0;
            for (auto &node : layers[0])
            {
                double input = 0;
                if (i < inputs.size())
                    input = inputs.at(i);
                node->SetOutput(input);
                ++i;
            }

            // Get output of last layer
            std::vector<double> outputs;
            for (auto &node : layers.back())
            {
                outputs.push_back(node->GetOutput());
            }

            std::cout << "Outputs: ";
            for (auto &output : outputs)
            {
                std::cout << output << " ";
            }
            std::cout << std::endl;

            // Choose the action with the highest output
            auto max_output = std::max_element(outputs.begin(), outputs.end());
            size_t index = std::distance(outputs.begin(), max_output);

            // If index is out of bounds, return the last action
            size_t action = 0;
            if (index >= actions.size())
                action = actions.back();
            else
                action = actions.at(index);
            std::cout << "Action: " << action << std::endl;
            return action;
        }

        void AddLayer(const std::vector<std::shared_ptr<GraphNode>> &layer) { layers.push_back(layer); }
    };
}
