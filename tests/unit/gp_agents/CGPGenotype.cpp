/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Group7_GP_Agent/CGPGenotype.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Group7_GP_Agent/CGPGenotype.hpp"
#include <ranges>

using namespace cowboys;
TEST_CASE("Genotype construction", "[group7][genotype]") {
  SECTION("Parameters constructor") {
    auto genotype = CGPGenotype({8, 4, 2, 10, 2});
    genotype.InitGenotype();
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (10 + 10) * 4);

    genotype = CGPGenotype({8, 4, 2, 10, 3});
    genotype.InitGenotype();
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (8 + 10 + 10) * 4);

    genotype = CGPGenotype({8, 4, 2, 10, 10});
    genotype.InitGenotype();
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (8 + 10 + 10) * 4);
  }
}
TEST_CASE("Genotype iterators", "[group7][genotype]") {
  SECTION("Genotype iterators") {
    CGPGenotype genotype({8, 4, 2, 10, 2});
    genotype.InitGenotype();
    auto it = genotype.begin();
    CHECK(it->input_connections.size() == 8);
    CHECK(it->function_idx == 0);
    CHECK((++it)->input_connections.size() == 8);

    it = genotype.begin();
    // Layer 1
    for (size_t i = 0; i < 10; ++i) {
      CHECK(it->input_connections.size() == 8);
      CHECK(it->function_idx == 0);
      ++it;
    }
    // Layer 2
    for (size_t i = 0; i < 10; ++i) {
      CHECK(it->input_connections.size() == 18);
      CHECK(it->function_idx == 0);
      ++it;
    }
    // Output layer
    for (size_t i = 0; i < 4; ++i) {
      CHECK(it->input_connections.size() == 20);
      CHECK(it->function_idx == 0);
      ++it;
    }
    CHECK(it == genotype.end());

    // All nodes should have 0 input connections when initialized by default
    bool all_0s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_0s = all_0s && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    CHECK(all_0s);
  }
}
TEST_CASE("Genotype mutation", "[group7][genotype]") {
  SECTION("Genotype mutation") {
    CGPGenotype genotype({10, 10, 200, 10, 10});
    genotype.InitGenotype();
    // Each connection will have a 0% chance of being mutated
    genotype.MutateConnections(0.);
    bool all_0s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_0s = all_0s && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    CHECK(all_0s);

    // Each connection will have a 100% chance of being mutated
    genotype.MutateConnections(1.);
    bool all_1s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_1s = all_1s && std::ranges::all_of(it->input_connections, [](char c) { return c == '1'; });
    CHECK(all_1s);

    // Each connection will have a 0% chance of being mutated
    genotype.MutateConnections(0.);
    all_1s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_1s = all_1s && std::ranges::all_of(it->input_connections, [](char c) { return c == '1'; });
    CHECK(all_1s);

    // Each connection will have a 50% chance of being mutated
    all_0s = true;
    all_1s = true;
    genotype.MutateConnections(0.5);
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_0s = all_0s && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
      all_1s = all_1s && std::ranges::all_of(it->input_connections, [](char c) { return c == '1'; });
    }
    // These tests could fail, but it's unlikely: chance of failure = 1 / (2 ^ 196500)
    CHECK_FALSE(all_0s);
    CHECK_FALSE(all_1s);
  }
}
TEST_CASE("base64", "[group7][base64]") {
  SECTION("ULL") {
    auto max_ull = std::numeric_limits<size_t>::max();
    // 64 bits for ull; 1 char represents 6 bits => 64 / 6 = 10.6666 => 11 chars
    // 64 bits set to all 1s => 10 chars of largest char + another char for left over bits
    // 64 % 6 = 4 => 4 bits left over => 4 bits of 1s => 16th char
    std::string max_encoded_ull = base64::chars.at(15) + std::string(10, base64::chars.at(63));
    CHECK(base64::ULLToB64(max_ull) == max_encoded_ull);
    CHECK(base64::B64ToULL(max_encoded_ull) == max_ull);
  }
  SECTION("Binary") {
    std::string max_encoded_ull = base64::chars.at(15) + std::string(10, base64::chars.at(63));
    CHECK(base64::B2ToB64(std::string(64, '1')) == max_encoded_ull);
    CHECK(base64::B64ToB2(max_encoded_ull) == std::string(64, '1'));
  }
}
TEST_CASE("Genotype configuration", "[group7][genotype]") {
  SECTION("Export as string") {
    // 2 input nodes, 1 output node. Only one node gene for the output node.
    auto genotype = CGPGenotype({2, 1, 0, 0, 2});
    CHECK(genotype.Export() == "2,1,0,0,2;0.0:");

    // 2 output nodes now, so 2 node genes
    genotype = CGPGenotype({2, 2, 0, 10, 3});
    CHECK(genotype.Export() == "2,2,0,10,3;0.0:0.0:");

    // back to 1 output node
    // 6 input nodes, still can represent with one base64 character
    genotype = CGPGenotype({6, 1, 0, 0, 2});
    CHECK(genotype.Export() == "6,1,0,0,2;0.0:");

    // 7 input nodes, now need 2 base64 characters
    genotype = CGPGenotype({7, 1, 0, 0, 2});
    CHECK(genotype.Export() == "7,1,0,0,2;00.0:");

    // 2 output nodes now
    genotype = CGPGenotype({7, 2, 0, 10, 3});
    CHECK(genotype.Export() == "7,2,0,10,3;00.0:00.0:");
  }
  SECTION("Configure by exported string") {
    std::string exported = "7,2,0,10,3;01.0:00.0:";
    auto genotype = CGPGenotype().Configure(exported);

    CHECK(genotype.GetNumInputs() == 7);
    CHECK(genotype.GetNumOutputs() == 2);
    CHECK(genotype.GetNumLayers() == 0);
    CHECK(genotype.GetNumNodesPerLayer() == 10);
    CHECK(genotype.GetLayersBack() == 3);

    // Connect all possible connections
    // ([1][63])_64 = (1 111111)_2
    std::string all_connected_7{base64::chars.at(1), base64::chars.at(63)};
    exported = std::format("7,2,0,10,3;{}.{}:{}.{}:", all_connected_7, base64::chars.at(1), all_connected_7,
                           base64::chars.at(20));
    genotype = CGPGenotype().Configure(exported);
    bool all_1s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_1s = all_1s && std::ranges::all_of(it->input_connections, [](char c) { return c == '1'; });
    CHECK(all_1s);
    CHECK(genotype.begin()->function_idx == 1);
    CHECK((++genotype.begin())->function_idx == 20);
  }
  SECTION("Exporting and configuration of random genotypes") {
    CGPGenotype genotype({8, 4, 10, 10, 2});
    genotype.MutateConnections(0.5);
    genotype.MutateFunctions(0.5, 100);

    auto genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);

    // Could fail, unlikely
    genotype2.MutateConnections(0.5);
    CHECK(genotype != genotype2);
  }
}