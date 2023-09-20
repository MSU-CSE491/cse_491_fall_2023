/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "core/WorldGrid.hpp"

TEST_CASE("WorldGrid Construction", "[core][grid]"){
  SECTION("Default construction"){
    cse491::WorldGrid grid;
    CHECK(grid.GetWidth() == 0);
    CHECK(grid.GetWidth() == 0);
    CHECK(grid.GetNumCells() == 0);
    CHECK(!grid.IsValid(0, 0));
    CHECK(!grid.IsValid(1, 1));
    CHECK(!grid.IsValid(-1, -1));
  }
  SECTION("Standard construction, no type"){
    cse491::WorldGrid grid(10, 10);
    CHECK(grid.GetWidth() == 10);
    CHECK(grid.GetWidth() == 10);
    CHECK(grid.GetNumCells() == 100);
    CHECK(grid.IsValid(0, 0));
    CHECK(grid.IsValid(1, 1));
    CHECK(grid.IsValid(9, 9));
    CHECK(!grid.IsValid(0, 10));
    CHECK(!grid.IsValid(10, 0));
    CHECK(!grid.IsValid(-1, -1));
  }
}
// TODO: Test resizing to a smaller size
// TODO: Check that resizing keeps the cells that are not lost / new
TEST_CASE("WorldGrid Resize", "[core][grid]"){
  SECTION("Resize to larger"){
    cse491::WorldGrid grid;
    CHECK(grid.GetWidth() == 0);
    CHECK(grid.GetWidth() == 0);
    CHECK(grid.GetNumCells() == 0);
    grid.Resize(10, 10);
    CHECK(grid.GetWidth() == 10);
    CHECK(grid.GetWidth() == 10);
    CHECK(grid.GetNumCells() == 100);
  }
}
