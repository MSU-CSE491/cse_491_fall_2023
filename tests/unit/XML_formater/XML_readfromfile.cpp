/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "tinyxml2.h"


// Use the TEST_CASE macro from Catch2 to define a test case.
TEST_CASE("TinyXML2 can read from a local file", "[xml][read]") {



//TODO: issues with saving at a specific location/directory and reading for a specific directory.
// works locally but not on github actions

//  tinyxml2::XMLDocument doc;
//
//
//  tinyxml2::XMLError result = doc.LoadFile("test.xml");
//
//
//  REQUIRE(result == tinyxml2::XML_SUCCESS);
//
//
//  tinyxml2::XMLElement* root = doc.FirstChildElement("root");
//  REQUIRE(root != nullptr);
//
//
//  tinyxml2::XMLElement* child = root->FirstChildElement("child");
//  REQUIRE(child != nullptr);
//
//
//  REQUIRE(std::string(child->GetText()) == "expected_value");


}
