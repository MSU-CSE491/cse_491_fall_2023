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
