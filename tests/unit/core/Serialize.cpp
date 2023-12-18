/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Serialize.hpp in source/core
 **/

#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project files
#include "core/GridPosition.hpp"
#include "core/ItemBase.hpp"
#include "core/Serialize.hpp"
#include "core/WorldGrid.hpp"

TEST_CASE("Serialize individual values", "[core][grid]"){
  std::stringstream ss;

  int i1 = 25;
  char c1 = '&';
  double d1 = 9.125;
  std::string s1 = "This is a test.";

  cse491::SerializeValue(ss, i1);
  cse491::SerializeValue(ss, c1);
  cse491::SerializeValue(ss, d1);
  cse491::SerializeValue(ss, s1);

  // Make sure original values have not changed.
  CHECK(i1 == 25);
  CHECK(c1 == '&');
  CHECK(d1 == 9.125);
  CHECK(s1 == "This is a test.");

  // Try restoring serialized values into a new set of variables.
  int i2 = 0;
  char c2 = '\0';
  double d2 = 0.0;
  std::string s2 = "";

  cse491::DeserializeValue(ss, i2);
  cse491::DeserializeValue(ss, c2);
  cse491::DeserializeValue(ss, d2);
  cse491::DeserializeValue(ss, s2);

  // Make sure restoration worked...
  CHECK(i2 == 25);
  CHECK(c2 == '&');
  CHECK(d2 == 9.125);
  CHECK(s2 == "This is a test.");

  // Try with some simple data structures.
  std::vector<std::string> v1{ "This", "is", "another", "test" };
  std::map<std::string, int> m1{{"ONE", 1}, {"TWO", 2}, {"THREE", 3}, {"FOUR", 4} };
  std::unordered_map<std::string, char> u1{{"Letter A", 'a'}, {"Letter B", 'b'}, {"Letter C", 'c'}, {"Letter D", 'd'} };

  // Spot check the data structures.
  CHECK(v1.size() == 4);
  CHECK(m1.size() == 4);
  CHECK(u1.size() == 4);
  CHECK(v1[2] == "another");
  CHECK(m1["TWO"] == 2);
  CHECK(u1["Letter A"] == 'a');

  cse491::SerializeValue(ss, v1);
  cse491::SerializeValue(ss, m1);
  cse491::SerializeValue(ss, u1);

  // Create new data structures to deserialize into...
  std::vector<std::string> v2;
  std::map<std::string, int> m2;
  std::unordered_map<std::string, char> u2;

  cse491::DeserializeValue(ss, v2);
  cse491::DeserializeValue(ss, m2);
  cse491::DeserializeValue(ss, u2);

  // Spot check the copies.
  CHECK(v2.size() == 4);
  CHECK(m2.size() == 4);
  CHECK(u2.size() == 4);
  CHECK(v2[2] == "another");
  CHECK(m2["TWO"] == 2);
  CHECK(u2["Letter A"] == 'a');

  // And make sure the copies are the same as the originals
  CHECK(v1 == v2);
  CHECK(m1 == m2);
  CHECK(u1 == u2);
}

TEST_CASE("Serialize Grids", "[core][grid]"){
  std::stringstream ss;

  cse491::WorldGrid grid1(100, 200, 3);  // Create a 100x200 grid initialzed to mostly threes.
  grid1.At(30,45) = 10;
  grid1.At(91,117) = 12345;

  // Spot check the grid.
  CHECK(grid1.GetNumCells() == 20000);
  CHECK(grid1.At(1,1) == 3);
  CHECK(grid1.At(2,2) == 3);
  CHECK(grid1.At(99,199) == 3);
  CHECK(grid1.At(30,45) == 10);
  CHECK(grid1.At(91,117) == 12345);

  cse491::SerializeValue(ss, grid1);

  // Make sure grid wasn't changed during serialization.
  CHECK(grid1.GetNumCells() == 20000);
  CHECK(grid1.At(1,1) == 3);
  CHECK(grid1.At(2,2) == 3);
  CHECK(grid1.At(99,199) == 3);
  CHECK(grid1.At(30,45) == 10);
  CHECK(grid1.At(91,117) == 12345);

  // Try restoring the grid into a new variable.
  cse491::WorldGrid grid2(2, 2);
  CHECK(grid2.GetNumCells() == 4);

  cse491::DeserializeValue(ss, grid2);
  
  CHECK(grid2.GetNumCells() == 20000);
  CHECK(grid2.At(1,1) == 3);
  CHECK(grid2.At(2,2) == 3);
  CHECK(grid2.At(99,199) == 3);
  CHECK(grid2.At(30,45) == 10);
  CHECK(grid2.At(91,117) == 12345);
}

TEST_CASE("Serialize Entities", "[core][grid]"){
  cse491::ItemBase item1(1, "Test Item");
  item1.SetPosition(10, 9);
  item1.SetProperties("health", 100.0, "alias", std::string("NotThis"), "uses", 10);

  CHECK(item1.GetID() == 1);
  CHECK(item1.GetName() == "Test Item");
  CHECK(item1.GetNumProperties() == 3);
  CHECK(item1.HasProperty("health") == true);
  CHECK(item1.HasProperty("alias") == true);
  CHECK(item1.HasProperty("uses") == true);
  CHECK(item1.HasProperty("damage") == false);
  CHECK(item1.HasProperty("damage") == false);
  CHECK(item1.HasProperty("fail") == false);
  CHECK(item1.GetProperty("health") == 100.0);
  CHECK(item1.GetProperty<std::string>("alias") == "NotThis");
  CHECK(item1.GetProperty<int>("uses") == 10);

  std::stringstream ss;
  cse491::SerializeValue(ss, item1);

  cse491::ItemBase item2(2, "New Item");
  CHECK(item2.GetID() == 2);
  CHECK(item2.GetName() == "New Item");
  CHECK(item2.GetNumProperties() == 0);

  cse491::DeserializeValue(ss, item2);
  CHECK(item2.GetID() == 1);
  CHECK(item2.GetName() == "Test Item");
  CHECK(item2.GetNumProperties() == 3);
  CHECK(item2.HasProperty("health") == true);
  CHECK(item2.HasProperty("alias") == true);
  CHECK(item2.HasProperty("uses") == true);
  CHECK(item2.HasProperty("damage") == false);
  CHECK(item2.HasProperty("damage") == false);
  CHECK(item2.HasProperty("fail") == false);
  CHECK(item2.GetProperty("health") == 100.0);
  CHECK(item2.GetProperty<std::string>("alias") == "NotThis");
  CHECK(item2.GetProperty<int>("uses") == 10);
}
