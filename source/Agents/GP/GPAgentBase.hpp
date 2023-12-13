/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief An Agent based on genetic programming.
 * @note Status: PROPOSAL
 **/

#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

#include "tinyxml2.h"

#include "../../core/AgentBase.hpp"

namespace cowboys {
  class GPAgentBase : public cse491::AgentBase {
  protected:
    std::unordered_map<std::string, double> extra_state; ///< A map of extra state information.
    unsigned int seed = 0;                               ///< Seed for the random number generator.
    std::mt19937 rng{seed};                              ///< Random number generator.
    std::uniform_real_distribution<double> uni_dist;     ///< Uniform distribution.
    std::normal_distribution<double> norm_dist;          ///< Normal distribution.

  public:
    GPAgentBase(size_t id, const std::string &name) : AgentBase(id, name) {
      Reset();
    }
    ~GPAgentBase() = default;

    /// @brief Setup graph.
    /// @return Success.
    bool Initialize() override { return true; }

    /// Choose the action to take a step in the appropriate direction.
    size_t SelectAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                        const cse491::item_map_t &item_set, const cse491::agent_map_t &agent_set) override {
      // Update extra state information before action
      if (extra_state["starting_x"] == std::numeric_limits<double>::max()) {
        auto pos = GetPosition();
        extra_state["starting_x"] = pos.GetX();
        extra_state["starting_y"] = pos.GetY();
      }

      size_t action = GetAction(grid, type_options, item_set, agent_set);

      // Update extra state information after action
      extra_state["previous_action"] = action;

      return action;
    }

    virtual size_t GetAction(const cse491::WorldGrid &grid, const cse491::type_options_t &type_options,
                             const cse491::item_map_t &item_set, const cse491::agent_map_t &agent_set) = 0;

    /// @brief Get a map of extra state information
    /// @return Map of extra state information
    const std::unordered_map<std::string, double> GetExtraState() const { return extra_state; }

    /// @brief Mutate this agent.
    /// @param mutation_rate The mutation rate. Between 0 and 1.
    virtual void MutateAgent(double mutation_rate = 0.8) = 0;

    /// @brief Copy the behavior of another agent into this agent.
    /// @param other The agent to copy. Should be the same type.
    virtual void Copy(const GPAgentBase &other) = 0;

    virtual void PrintAgent(){

    };

    virtual void SerializeGP(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *parentElem, double fitness = -1) = 0;

    virtual std::string Export() { return ""; }

    virtual void Reset(bool /*hard*/ = false) {
      extra_state["previous_action"] = 0;
      extra_state["starting_x"] = std::numeric_limits<double>::max();
      extra_state["starting_y"] = std::numeric_limits<double>::max();
    };

    //    virtual void crossover(const GPAgentBase &other) {};
    virtual void Import(const std::string &genotype) = 0;

    // -- Random Number Generation --

    /// @brief  Set the seed used to initialize this RNG
    void SetSeed(unsigned int seed) {
      this->seed = seed;
      rng.seed(seed);
    }

    /// @brief  Get the seed used to initialize this RNG
    unsigned int GetSeed() const { return seed; }

    /// @brief  Return a uniform random value between 0.0 and 1.0
    double GetRandom() { return uni_dist(rng); }

    /// @brief  Return a uniform random value between 0.0 and max
    double GetRandom(double max) { return GetRandom() * max; }

    /// @brief  Return a uniform random value between min and max
    double GetRandom(double min, double max) {
      assert(max > min);
      return min + GetRandom(max - min);
    }

    /// @brief  Return a uniform random unsigned long long between 0 (inclusive) and max (exclusive)
    size_t GetRandomULL(size_t max) { return static_cast<size_t>(GetRandom(max)); }

    /// @brief  Return a gaussian random value with mean 0.0 and sd 1.0
    double GetRandomNormal() { return norm_dist(rng); }

    /// @brief  Return a gaussian random value with provided mean and sd.
    double GetRandomNormal(double mean, double sd = 1.0) {
      assert(sd > 0);
      return mean + norm_dist(rng) * sd;
    }
  };

} // End of namespace cowboys
