/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "tinyxml2.h"


TEST_CASE("TinyXML2 can parse from string", "[xml][parse]") {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError result = doc.Parse("<root><child>value</child></root>");

  REQUIRE(result == tinyxml2::XML_SUCCESS);

  tinyxml2::XMLElement* root = doc.FirstChildElement("root");
  REQUIRE(root != nullptr);

  tinyxml2::XMLElement* child = root->FirstChildElement("child");
  REQUIRE(child != nullptr);
  REQUIRE(std::string(child->GetText()) == "value");
}
