/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on genetic programming.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <string>

#include "GPAgentBase.hpp"
#include "GraphBuilder.hpp"

namespace cowboys {
  /// Don't know the maximum size a state can be, arbitrary large number
  constexpr size_t INPUT_SIZE = 6;

  /// Number of computational layers for each agent
  constexpr size_t NUM_LAYERS = 3;

  /// The number of nodes in each layer
  constexpr size_t NUM_NODES_PER_LAYER = 5;

  /// The number of layers preceding a node's layer that the node can reference
  constexpr size_t LAYERS_BACK = 2;

  /// @brief An agent based on cartesian genetic programming.
  class CGPAgent : public GPAgentBase {
  protected:
    /// The genotype for this agent.
    CGPGenotype genotype;

    /// The decision graph for this agent.
    std::unique_ptr<Graph> decision_graph;



  public:
    CGPAgent(size_t id, const std::string &name) : GPAgentBase(id, name) {}
    CGPAgent(size_t id, const std::string &name, const CGPGenotype &genotype)
        : GPAgentBase(id, name), genotype(genotype) {}


    void PrintAgent() override {
      std::cout << "Genotype: " << genotype.Export() << std::endl;
    }

    void MutateAgent(double mutation = 0.8) override {
      auto graph_builder = GraphBuilder();

      genotype.MutateDefault(mutation, *this);

      decision_graph = graph_builder.CartesianGraph(genotype, FUNCTION_SET, this);
    }
    /// @brief Setup graph.
    /// @return Success.
    bool Initialize() override {

      // Create a default genotype if none was provided in the constructor
      if (genotype.GetNumFunctionalNodes() == 0) {
        genotype = CGPGenotype({INPUT_SIZE, action_map.size(), NUM_LAYERS, NUM_NODES_PER_LAYER, LAYERS_BACK});
      }

      genotype.SetSeed(rand());

      // Mutate the beginning genotype, might not want this.
      MutateAgent(0.2);

      return true;
    }

    size_t GetAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                     const cse491::item_map_t &item_set, const cse491::agent_map_t &agent_set) override {
      auto inputs = EncodeState(grid, type_options, item_set, agent_set, this, extra_state);
      size_t action_to_take = decision_graph->MakeDecision(inputs, EncodeActions(action_map));
      return action_to_take;
    }


    void Serialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElem, int fitness = -1) override {
      auto agentElem = doc.NewElement("CGPAgent");
      parentElem->InsertEndChild(agentElem);

      auto genotypeElem = doc.NewElement("genotype");
      genotypeElem->SetText(genotype.Export().c_str());
      if (fitness != -1)
        genotypeElem->SetAttribute("fitness", fitness);
      agentElem->InsertEndChild(genotypeElem);

    }


    /// @brief Get the genotype for this agent.
    /// @return A const reference to the genotype for this agent.
    const CGPGenotype &GetGenotype() const { return genotype; }


    /// @brief Copies the genotype and behavior of another CGPAgent into this agent.
    /// @param other The CGPAgent to copy.
    void Configure(const CGPAgent &other) {
      genotype = other.GetGenotype();
      decision_graph = GraphBuilder().CartesianGraph(genotype, FUNCTION_SET, this);
    }

    /// @brief Copy the behavior of another agent into this agent.
    /// @param other The agent to copy.
    void Copy(const GPAgentBase &other) override {
      assert(dynamic_cast<const CGPAgent *>(&other) != nullptr);
      Configure(dynamic_cast<const CGPAgent &>(other));
    }
  };

} // End of namespace cowboys
