//
// Created by Matthew Kight on 9/24/23.
//
#pragma once

#include "../core/AgentBase.hpp"
#include "../core/WorldBase.hpp"
#include <vector>
#include <map>
#include <queue>
#include <tuple>

namespace walle
{
    /**
     * @brief Utility Function to calculate the 'h' heuristics.
     *
     * @param start point we are starting at
     * @param end  point we want to get to
     * @return double euclidian distance between two points
     */
    double calculateHValue(cse491::GridPosition start, cse491::GridPosition end)
    {
        // Return using the distance formula
        return ((double)sqrt(
            (start.GetX() - end.GetX()) * (start.GetX() - end.GetX()) + (start.GetY() - end.GetY()) * (start.GetY() - end.GetY())));
    }

    /**
     * @brief Node class to make A* search easier
     *
     */
    struct Node
    {
        cse491::GridPosition position; // Where node is located
        int g;                         // Cost from start to current node
        int h;                         // Heuristic (estimated cost from current node to goal)
        Node *parent;

        Node(cse491::GridPosition position, double g, double h, Node *parent)
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
        bool operator()(const Node *a, const Node *b) const
        {
            return a->f() > b->f();
        }
    };

    /// @brief Uses A* to return a  list of grid positions
    /// @param start Starting position for search
    /// @param end Ending position for the search
    /// @return vector of A* path from start to end, empty vector if no path
    /// exist
    std::vector<cse491::GridPosition> cse491::WorldBase::shortest_path(cse491::GridPosition start, cse491::GridPosition end)
    {
        // TODO remove the use of new

        // Generated with the help of chat.openai.com
        const int rows = this->main_grid.GetWidth();
        const int cols = this->main_grid.GetHeight();
        std::vector<cse491::GridPosition> path;
        // If the start or end is not valid then return empty list
        if (!(this->main_grid.IsValid(start) && this->main_grid.IsValid(end)))
            return path;

        // Define possible movements (up, down, left, right)
        const int dx[] = {-1, 1, 0, 0};
        const int dy[] = {0, 0, -1, 1};

        // Create a 2D vector to store the cost to reach each cell
        std::vector<std::vector<int>> cost(rows, std::vector<int>(cols, INT_MAX));

        // Create an open list as a priority queue
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> openList;

        // Create the start and end nodes
        auto startNode = std::make_shared<Node>(start, 0, 0, nullptr);
        auto endNode = std::make_shared<Node>(end, 0, 0, nullptr);

        openList.push(startNode);
        cost[start.GetX()][start.GetY()] = 0;

        while (!openList.empty())
        {
            auto current = openList.top();
            openList.pop();

            if (current->position == endNode->position)
            {
                auto location = current.get();
                // Reached the goal, reconstruct the path
                while (location != nullptr)
                {
                    path.push_back(current->position);
                    location = location->parent;
                }
                break;
            }

            // Explore the neighbors
            for (int i = 0; i < 4; ++i)
            {
                cse491::GridPosition newPos(current->position.GetX() + dx[i], current->position.GetY() + dy[i]);
                // Check if the neighbor is within bounds and is a valid move
                if (this->main_grid.IsValid(newPos) && this->is_walkable(newPos))
                {
                    int newG = current->g + 1;                                                                                          // Assuming a cost of 1 to move to a neighbor
                    int newH = std::abs(newPos.GetX() - endNode->position.GetX()) + std::abs(newPos.GetY() - endNode->position.GetY()); // Manhattan distance

                    if (newG < cost[newPos.GetX()][newPos.GetY()])
                    {
                        auto neighbor = std::make_shared<Node>(newPos, newG, newH, current);
                        openList.push(neighbor);
                        cost[newPos.GetX()][newPos.GetY()] = newG;
                    }
                }
            }
        }

        return path;
    }

}