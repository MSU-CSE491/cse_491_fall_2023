//
// Created by Matthew Kight on 9/24/23.
//
#pragma once
#include "../core/AgentBase.hpp"
#include <vector>
#include <map>
#include <queue>
#include <tuple>
namespace cse491
{
    namespace walle
    {

        /**
         * @brief Node class to make A* search easier
         *
         */
        struct Node
        {
            cse491::GridPosition position; // Where node is located
            int g;                         // Cost from start to current node
            int h;                         // Heuristic (estimated cost from current node to goal)
            std::shared_ptr<Node> parent;

            Node(cse491::GridPosition position, double g, double h, std::shared_ptr<Node> parent)
                : position(position), g(g), h(h), parent(parent) {}

            // Calculate the total cost (f) of the node
            int f() const
            {
                return g + h;
            }
        };

        /**
         * @brief Custom comparison function for priority queue
         *
         */
        struct CompareNodes
        {
            bool operator()(const std::shared_ptr<walle::Node> a, const std::shared_ptr<walle::Node> b) const
            {
                return a->f() > b->f();
            }
        };

    }
}
