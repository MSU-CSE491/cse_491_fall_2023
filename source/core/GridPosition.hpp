/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A mechanism of identifying a grid cell, as well as a position within the cell.
 * @note Status: PROPOSAL
 **/


#pragma once

#include <cassert>
#include <compare>    // For operator<=>
#include <cstddef>    // For size_t
#include <cmath>      // For sqrt and std::nan()

namespace cse491 {

  /// @class GridPosition
  /// @brief Represents a position within a 2D grid of cells.
  /// This class provides utilities to manage a position in 2D space.
  /// The position is stored as floating-point values (to allow for smooth motion through
  /// a grid), but is easily converted to size_t for grid-cell identification.
  class GridPosition {
  private:
    double x = 0.0;
    double y = 0.0;

  public:
    GridPosition() = default;
    GridPosition(double x, double y) : x(x), y(y) { }
    GridPosition(const GridPosition &) = default;

    GridPosition & operator=(const GridPosition &) = default;

    // -- Accessors --

    [[nodiscard]] double GetX() const { return x; }
    [[nodiscard]] double GetY() const { return y; }
    [[nodiscard]] size_t CellX() const { return static_cast<size_t>(x); }
    [[nodiscard]] size_t CellY() const { return static_cast<size_t>(y); }

    /// Enable all comparison operators (==, !=, <, <=, >, >=)
    auto operator<=>(const GridPosition &) const = default;

    [[nodiscard]] bool IsValid() const { return !(std::isnan(x) || std::isnan(y)); }

    // -- Modifiers --

    GridPosition & Set(double in_x, double in_y) {
      x=in_x; y=in_y;
      return *this;
    }
    GridPosition & Shift(double shift_x, double shift_y) {
      x += shift_x; y += shift_y;
      return *this;
    }

    GridPosition & operator+=(const GridPosition & in) { return Shift(in.x, in.y); }
    GridPosition & operator-=(const GridPosition & in) { return Shift(-in.x, -in.y); }

    GridPosition & MakeInvalid() { x = y = std::nan("NAN(0)"); return *this; }

    // -- Const Operations --

    /// Return a the GridPosition at the requested offset.
    [[nodiscard]] GridPosition GetOffset(double offset_x, double offset_y) const {
      return GridPosition{x+offset_x,y+offset_y};
    }

    /// Return a grid position above this one (by default, directly above)
    [[nodiscard]] GridPosition Above(double dist=1.0) const { return GetOffset(0.0, -dist); }

    /// Return a grid position below this one (by default, directly below)
    [[nodiscard]] GridPosition Below(double dist=1.0) const { return GetOffset(0.0, dist); }

    /// Return a grid position to the left of this one (by default, directly left)
    [[nodiscard]] GridPosition ToLeft(double dist=1.0) const { return GetOffset(-dist, 0.0); }

    /// Return a grid position to the right of this one (by default, directly right)
    [[nodiscard]] GridPosition ToRight(double dist=1.0) const { return GetOffset(dist, 0.0); }

    /// Add together two grid positions and return the result.
    [[nodiscard]] GridPosition operator+(GridPosition in) const {
      return GetOffset(in.x, in.y);
    }

    [[nodiscard]] double Distance(GridPosition pos2) const {
      const double dist1 = x - pos2.x;
      const double dist2 = y - pos2.y;
      return sqrt(dist1*dist1 + dist2*dist2);
    }

    /// @brief  Manhattan distance between grid positions
    /// @param pos2 Position to compare to
    /// @return Manhattan distance
    [[nodiscard]] double MDistance(GridPosition pos2) const {
      const double dist1 = x - pos2.x;
      const double dist2 = y - pos2.y;
      return abs(dist1) + abs(dist2);
    }

    [[nodiscard]] bool IsNear(GridPosition pos2, double max_dist=1.0) const {
      const double dist1 = x - pos2.x;
      const double dist2 = y - pos2.y;
      return (dist1*dist1 + dist2*dist2) <= (max_dist * max_dist);
    }
  };

} // End of namespace cse491
