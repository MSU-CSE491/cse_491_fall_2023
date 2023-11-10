/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Entity.hpp in source/core
 * @author Jayson Van Dam
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
// Place your includes here
// e.g., #include "core/Data.hpp"
#include "core/Entity.hpp"

TEST_CASE("Entity Construction", "[core][entity]") {
  SECTION("Construction with ID and Name") {
    cse491::Entity entity(1, "Axe");
    cse491::GridPosition position = entity.GetPosition();

    CHECK(entity.GetID() == 1);
    CHECK(entity.GetName() == "Axe");

    CHECK(position.GetX() == 0.0);
    CHECK(position.GetY() == 0.0);
  }
}

TEST_CASE("Entity Serialization", "[core][entity]") {
  cse491::Entity entity(1, "Axe");
  const std::string expected =
      "{\"entity_x\":0.0,\"entity_y\":0.0,\"name\":\"Axe\"}";
  std::string result = entity.Serialize();
  CHECK(result == expected);
}
