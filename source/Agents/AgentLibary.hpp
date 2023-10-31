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

namespace walle {

/**
 * @brief Node class to hold information about positions for A* search
 */
struct Node {
  cse491::GridPosition position;    ///< Where node is located
  double g;                         ///< Cost from start to current node
  double h;                         ///< Heuristic (estimated cost from current node to goal)
  std::shared_ptr<Node> parent;     ///< How we got to this node (Used to construct final path)

  /**
   * Constructor for a node
   * @param position location on grid of this node
   * @param g actual distance to get from start to this node
   * @param h heuristic guess for distance from this node to end location
   * @param parent Used to construct path back at end
   */
  Node(const cse491::GridPosition &position, double g, double h, std::shared_ptr<Node> parent)
      : position(position), g(g), h(h), parent(std::move(parent)) {}

  /**
   * @brief Calculate the total cost (f) of the node
   * @return sum of actual distance and heuristic distance
   */
  [[nodiscard]] double f() const {
    return g + h;
  }
};

/**
 * @brief Custom comparison function for priority queue
 * @return if a has a greater total cost than b
 */
struct CompareNodes {
  bool operator()(const std::shared_ptr<walle::Node> &a, const std::shared_ptr<walle::Node> &b) const {
    return a->f() > b->f();
  }
};

/// @brief Uses A* to return a  list of grid positions
/// @author @mdkdoc15
/// @param start Starting position for search
/// @param end Ending position for the search
/// @return vector of A* path from start to end, empty vector if no path
/// exist
std::vector<cse491::GridPosition> GetShortestPath(const cse491::GridPosition &start,
                                                  const cse491::GridPosition &end,
                                                  const cse491::WorldBase &world,
                                                  const cse491::AgentBase &agent) {
  // Generated with the help of chat.openai.com
  const size_t rows = world.GetGrid().GetWidth();
  const size_t cols = world.GetGrid().GetHeight();
  std::vector<cse491::GridPosition> path;
  // If the start or end is not valid then return empty list
  if (!(world.GetGrid().IsValid(start) && world.GetGrid().IsValid(end)))
    return path;

  // Define possible movements (up, down, left, right)
  const int dx[] = {-1, 1, 0, 0};
  const int dy[] = {0, 0, -1, 1};

  // Create a 2D vector to store the cost to reach each cell
  std::vector<std::vector<double>> cost(rows, std::vector<double>(cols, INT_MAX));

  // Create an open list as a priority queue
  std::priority_queue<std::shared_ptr<walle::Node>, std::vector<std::shared_ptr<walle::Node>>, walle::CompareNodes>
      openList;

  // Create the start and end nodes
  auto startNode = std::make_shared<walle::Node>(start, 0, 0, nullptr);
  auto endNode = std::make_shared<walle::Node>(end, 0, 0, nullptr);

  openList.push(startNode);
  cost[start.CellX()][start.CellY()] = 0;

  while (!openList.empty()) {
    auto current = openList.top();
    openList.pop();

    if (current->position == endNode->position) {

      // Reached the goal, reconstruct the path
      while (current != nullptr) {
        path.push_back(current->position);
        current = current->parent;
      }
      break;
    }

    // Explore the neighbors
    for (int i = 0; i < 4; ++i) {
      cse491::GridPosition newPos(current->position.GetX() + dx[i], current->position.GetY() + dy[i]);
      // Check if the neighbor is within bounds and is a valid move
      if (world.GetGrid().IsValid(newPos) && world.IsTraversable(agent, newPos)) {
        double newG = current->g + 1;                   // Assuming a cost of 1 to move to a neighbor
        double newH = std::abs(newPos.GetX() - endNode->position.GetX()) +
            std::abs(newPos.GetY() - endNode->position.GetY()); // Manhattan distance

        if (newG + newH < cost[newPos.CellX()][newPos.CellY()]) {
          auto neighbor = std::make_shared<walle::Node>(newPos, newG, newH, current);
          openList.push(neighbor);
          cost[newPos.CellX()][newPos.CellY()] = newG + newH;
        }
      }
    }
  }

  return path;
}

}
