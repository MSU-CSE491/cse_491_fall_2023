/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "core/Data.hpp"
#include "core/Entity.hpp"
#include "core/AgentBase.hpp"

TEST_CASE("CellType", "[core]"){
  cse491::CellType cell_type{"name", "desc", '@'};
  CHECK(cell_type.name == "name");
  CHECK(cell_type.desc == "desc");
  CHECK(cell_type.symbol == '@');
}

TEST_CASE("CellType properties", "[core]"){
  // Regular values test
  cse491::CellType cell_type{"test", "test description", '^'};
  CHECK(cell_type.name == "test");
  CHECK(cell_type.desc == "test description");
  CHECK(cell_type.symbol == '^');
  // Add properties
  cell_type.SetProperty("prop1");
  cell_type.SetProperty("prop2");
  cell_type.SetProperty("prop3");
  // Check properties were added
  CHECK(cell_type.HasProperty("prop1"));
  CHECK(cell_type.HasProperty("prop2"));
  CHECK(cell_type.HasProperty("prop3"));
  // Remove properties
  cell_type.RemoveProperty("prop2");
  cell_type.RemoveProperty("prop3");
  // Check properties were removed
  CHECK(cell_type.HasProperty("prop1"));
  CHECK_FALSE(cell_type.HasProperty("prop2"));
  CHECK_FALSE(cell_type.HasProperty("prop3"));
  // Chaining adds and removals
  cell_type.RemoveProperty("prop1").SetProperty("prop3").SetProperty("prop4");
  // Check that changes worked
  CHECK_FALSE(cell_type.HasProperty("prop1"));
  CHECK_FALSE(cell_type.HasProperty("prop2"));
  CHECK(cell_type.HasProperty("prop3"));
  CHECK(cell_type.HasProperty("prop4"));
  
  
}
