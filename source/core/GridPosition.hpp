/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A mechanism of identifying a grid cell, as well as a position within the cell.
 * @note Status: PROPOSAL
 **/


#pragma once

#include <cassert>
#include <compare>    // For operator<=>
#include <cstddef>    // For size_t

namespace cse491 {

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

    // -- Modifiers --

    GridPosition & Set(double in_x, double in_y) {
      assert(in_x > 0 && in_y > 0);
      x=in_x; y=in_y;
      return *this;
    }
    GridPosition & Shift(double x_shift, double y_shift) {
      x += x_shift; y += y_shift;
      return *this;
    }

    GridPosition & operator+=(const GridPosition & in) { return Shift(in.x, in.y); }
    GridPosition & operator-=(const GridPosition & in) { return Shift(-in.x, -in.y); }


    // -- Const Operations --
    [[nodiscard]] GridPosition operator+(GridPosition in) const { return in.Shift(x,y); }

    [[nodiscard]] GridPosition GetOffset(double x_offset, double y_offset) const {
      return GridPosition{x+x_offset,y+y_offset};
    }
    [[nodiscard]] GridPosition Above(double dist=1.0) const { return GetOffset(0.0, -dist); }
    [[nodiscard]] GridPosition Below(double dist=1.0) const { return GetOffset(0.0, dist); }
    [[nodiscard]] GridPosition ToLeft(double dist=1.0) const { return GetOffset(-dist, 0.0); }
    [[nodiscard]] GridPosition ToRight(double dist=1.0) const { return GetOffset(dist, 0.0); }
  };

} // End of namespace cse491
