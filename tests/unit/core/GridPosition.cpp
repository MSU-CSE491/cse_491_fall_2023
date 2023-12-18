/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for GridPosition.hpp in source/core
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "core/GridPosition.hpp"

TEST_CASE("GridPosition Construction", "[core][grid]") {
  cse491::GridPosition default_pos;
  CHECK(default_pos.GetX() == 0.0);
  CHECK(default_pos.GetY() == 0.0);
  CHECK(default_pos.CellX() == 0);
  CHECK(default_pos.CellY() == 0);
  CHECK(default_pos.IsValid() == true);
  CHECK(default_pos.IsInvalid() == false);

  cse491::GridPosition zero_pos(0.0, 0.0);
  CHECK(zero_pos.GetX() == 0.0);
  CHECK(zero_pos.GetY() == 0.0);
  CHECK(zero_pos.CellX() == 0);
  CHECK(zero_pos.CellY() == 0);
  CHECK(zero_pos.IsValid() == true);
  CHECK(zero_pos.IsInvalid() == false);
  CHECK(zero_pos == default_pos);

  cse491::GridPosition set_pos(3.0, 4.0);
  CHECK(set_pos.GetX() == 3.0);
  CHECK(set_pos.GetY() == 4.0);
  CHECK(set_pos.CellX() == 3);
  CHECK(set_pos.CellY() == 4);
  CHECK(set_pos.IsValid() == true);
  CHECK(set_pos.IsInvalid() == false);
  CHECK(set_pos != default_pos);

  cse491::GridPosition copy_pos(set_pos);
  CHECK(copy_pos.GetX() == 3.0);
  CHECK(copy_pos.GetY() == 4.0);
  CHECK(copy_pos.CellX() == 3);
  CHECK(copy_pos.CellY() == 4);
  CHECK(copy_pos.IsValid() == true);
  CHECK(copy_pos.IsInvalid() == false);
  CHECK(copy_pos != default_pos);
  CHECK(copy_pos == set_pos);
}

TEST_CASE("GridPosition Mathematical Methods", "[core][grid]") {
  cse491::GridPosition pos1(0.0, 0.0);
  cse491::GridPosition pos2(3.0, 4.0);
  cse491::GridPosition pos3(1.0, 2.0);

  pos1.Set(7.0,7.0);
  CHECK(pos1.GetX() == 7.0);
  CHECK(pos1.GetY() == 7.0);

  pos1.Shift(-7.0,1.0);
  CHECK(pos1.GetX() == 0.0);
  CHECK(pos1.GetY() == 8.0);

  pos1 += pos2;
  CHECK(pos1.GetX() == 3.0);
  CHECK(pos1.GetY() == 12.0);

  pos1 -= pos3;
  CHECK(pos1.GetX() == 2.0);
  CHECK(pos1.GetY() == 10.0);

  pos1.MakeInvalid();
  CHECK(pos1.IsInvalid() == true);
  CHECK(pos1.IsValid() == false);

  pos1 = pos2.GetOffset(10.0, 20.0);
  CHECK(pos1.GetX() == 13.0);
  CHECK(pos1.GetY() == 24.0);

  CHECK(pos1.Above().GetX() == 13.0);
  CHECK(pos1.Above().GetY() == 23.0);
  CHECK(pos1.Below().GetX() == 13.0);
  CHECK(pos1.Below().GetY() == 25.0);
  CHECK(pos1.ToLeft().GetX() == 12.0);
  CHECK(pos1.ToLeft().GetY() == 24.0);
  CHECK(pos1.ToRight().GetX() == 14.0);
  CHECK(pos1.ToRight().GetY() == 24.0);

  pos1 = pos2 + pos3;
  CHECK(pos1.GetX() == 4.0);
  CHECK(pos1.GetY() == 6.0);

  double dist = pos1.Distance(pos3);
  CHECK(dist > 4.999);
  CHECK(dist < 5.001);

  dist = pos1.MDistance(pos3); // Manhattan distance
  CHECK(dist > 6.999);
  CHECK(dist < 7.001);

  CHECK(pos1.IsNear(pos3) == false);
  CHECK(pos1.IsNear(pos3, 4.9) == false);
  CHECK(pos1.IsNear(pos3, 5.0) == true);
  CHECK(pos1.IsNear(pos3, 5.1) == true);
}

TEST_CASE("GridPosition String Methods", "[core][grid]") {
  cse491::GridPosition pos1(0.0, 0.0);
  cse491::GridPosition pos2(3.0, 4.0);
  cse491::GridPosition pos3(1.0, 2.0);

  CHECK(pos1.ToString() == "(0,0)");
  CHECK(pos2.ToString() == "(3,4)");
  CHECK(pos3.ToString() == "(1,2)");

  pos1.FromString("(10,11)");
  pos2.FromString("(12345,67890)");
  pos3.FromString("(-1.0,2.5)");
  CHECK(pos1.GetX() == 10.0);
  CHECK(pos1.GetY() == 11.0);
  CHECK(pos2.GetX() == 12345.0);
  CHECK(pos2.GetY() == 67890.0);
  CHECK(pos3.GetX() == -1.0);
  CHECK(pos3.GetY() == 2.5);

  CHECK(pos1.ToString() == "(10,11)");
  CHECK(pos2.ToString() == "(12345,67890)");
  CHECK(pos3.ToString() == "(-1,2.5)");

  pos1.FromString("(10,11");
  pos2.FromString("(1234567890)");
  pos3.FromString("-1.0,2.5)");
  CHECK(pos1.IsValid() == false);
  CHECK(pos2.IsValid() == false);
  CHECK(pos3.IsValid() == false);
}
