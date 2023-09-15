/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simple 2D Grid container
 * @note Status: PROPOSAL
 **/


#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "GridPosition.hpp"

namespace cse491 {

  class WorldGrid {
  protected:
    size_t width;
    size_t height;
    std::vector<size_t> cells;  ///< All cells, grouped by full rows, top to bottom

    // @CAO Question: Should we add an 'offset' or other mechanism for a grid to
    //                more explicitly represent a portion of a larger grid?

    // -- Helper functions --

    /// Convert an X and a Y value to the index in the vector.
    [[nodiscard]] inline size_t ToIndex(size_t x, size_t y) const {
      return x + y * width;
    }

  public:
    WorldGrid(size_t width=0, size_t height=0, size_t default_type=0)
      : width(width), height(height), cells(width*height, default_type) { }
    WorldGrid(const WorldGrid &) = default;
    WorldGrid(WorldGrid &&) = default;
    
    WorldGrid & operator=(WorldGrid &&) = default;

    // -- Accessors --
    [[nodiscard]] size_t GetWidth() const { return width; }
    [[nodiscard]] size_t GetHeight() const { return height; }
    [[nodiscard]] size_t GetNumCells() const { return cells.size(); }

    /// Test if specific coordinates are in range for this GridWorld.
    [[nodiscard]] bool IsValid(size_t x, size_t y) const {
      return x < width && y < height;
    }

    /// Test if a GridPosition is in range for this GridWorld.
    [[nodiscard]] bool IsValid(GridPosition pos) const {
      return IsValid(pos.CellX(), pos.CellY());
    }

    /// @return The grid state at the provided x and y coordinates
    [[nodiscard]] size_t At(size_t x, size_t y) const {
      assert(IsValid(x,y));
      return cells[ToIndex(x,y)];
    }

    /// @return A reference to the grid state at the provided x and y coordinates
    [[nodiscard]] size_t & At(size_t x, size_t y) {
      assert(IsValid(x,y));
      return cells[ToIndex(x,y)];
    }

    /// @return The state at a given GridPosition.
    [[nodiscard]] size_t At(GridPosition pos) const {
      assert(IsValid(pos));
      return At(pos.CellX(), pos.CellY());
    }

    /// @return A reference to the state at a given GridPosition.
    [[nodiscard]] size_t & At(GridPosition pos) {
      assert(IsValid(pos));
      return At(pos.CellX(), pos.CellY());
    }

    [[nodiscard]] size_t operator[](GridPosition pos) const { return At(pos); }
    [[nodiscard]] size_t & operator[](GridPosition pos) { return At(pos); }


    // Size adjustments.
    void Resize(size_t new_width, size_t new_height, size_t default_type=0) {
      // Create a new vector of the correct size.
      std::vector<size_t> new_cells(new_width*new_height, default_type);

      // Copy the overlapping portions of the two grids.
      size_t min_width = std::min(width, new_width);
      size_t min_height = std::min(height, new_height);
      for (size_t x = 0; x < min_width; ++x) {
        for (size_t y = 0; y < min_height; ++y) {
          new_cells[x+y*new_width] = cells[ToIndex(x,y)];
        }
      }

      // Swap the new grid in; let the old grid be deallocated in its place.
      std::swap(cells, new_cells);
      width = new_width;
      height = new_height;
    }

    // -- Save and Load functions --
    // Mechanisms to efficiently save and load the exact state of the file.
    // File format is width and height followed by all
    // values in the grid on each line thereafter.

    /// Write the current state of this grid into the provided stream.
    void Save(std::ostream & os) const {
      os << width << " " << height;
      for (size_t state : cells) os << ' ' << state;
      os << std::endl;
    }

    /// Helper function to specify a file name to write the grid state to.
    void Save(std::string filename) const {
      std::ofstream os(filename);
      Save(os);
    }

    /// Read the state of the grid out of the provided stream. 
    void Load(std::istream & is) {
      is >> width >> height;
      cells.resize(width * height);
      for (size_t & state : cells) is >> state;
    }

    /// Helper function to specify a file name to load the grid state from.
    void Load(std::string filename) {
      std::ifstream is(filename);
      Load(is);
    }

  };

} // End of namespace cse491