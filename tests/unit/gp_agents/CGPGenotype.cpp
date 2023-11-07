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
    // Each node can look back 2 layers
    // 1st layer has 10 nodes, 8 connections each to the inputs
    // 2nd layer has 10 nodes, 8 connections to the inputs + 10 connections each to the 1st layer
    // Output layer has 4 nodes, 10 connections each to the 2nd layer + 10 connections each to the 1st layer
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (10 + 10) * 4);

    genotype = CGPGenotype({8, 4, 2, 10, 3});
    // Each node can look back 3 layers
    // 1st layer has 10 nodes, 8 connections each to the inputs
    // 2nd layer has 10 nodes, 10 connections each to the 1st layer + 8 connections to the inputs
    // Output layer has 4 nodes, 10 connections each to the 2nd layer + 10 connections each to the 1st layer + 8
    // connections to the inputs
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (8 + 10 + 10) * 4);

    genotype = CGPGenotype({8, 4, 2, 10, 10});
    // Each node can look back 10 layers, but there are only 4 layers so each layer can look backwards all layers
    CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (8 + 10 + 10) * 4);
  }
}
TEST_CASE("Genotype iterators", "[group7][genotype]") {
  SECTION("Genotype iterators") {
    CGPGenotype genotype({8, 4, 2, 10, 2});
    auto it = genotype.begin();
    // We ignore the input nodes, so the first node should have 8 input connections
    CHECK(it->input_connections.size() == 8);

    // Iterate through all nodes, checking input connections to differentiate them
    it = genotype.begin();
    // Layer 1
    for (size_t i = 0; i < 10; ++i) {
      CHECK(it->input_connections.size() == 8);
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
  // Graph should be big enough to reduce chance of false positives
  CGPGenotype genotype({10, 10, 200, 10, 10});
  SECTION("Mutate connections") {
    // Each connection will have a 0% chance of being mutated
    genotype.MutateConnections(0.);
    bool all_0s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_0s = all_0s && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    CHECK(all_0s);

    // Each connection will have a 100% chance of being mutated
    genotype.MutateConnections(1.);
    all_0s = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it)
      all_0s = all_0s && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    CHECK_FALSE(all_0s);
  }
  SECTION("Mutate functions") {
    bool all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->function_idx == 0;
    }
    CHECK(all_default);

    // Nothing should change
    genotype.MutateFunctions(0., 100);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->function_idx == 0;
    }
    CHECK(all_default);

    // Most should change
    genotype.MutateFunctions(1., 100);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->function_idx == 0;
    }
    CHECK_FALSE(all_default);
  }
  SECTION("Mutate outputs") {
    bool all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
    }
    CHECK(all_default);

    // Nothing should change
    genotype.MutateOutputs(0., -100, 100);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
    }
    CHECK(all_default);

    // Should change
    genotype.MutateOutputs(1., -100, 100);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
    }
    CHECK_FALSE(all_default);
  }
  SECTION("Mutate default") {
    bool all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
      all_default = all_default && it->function_idx == 0;
      all_default = all_default && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    }
    CHECK(all_default);

    // Mutate with 0 probability, nothing should change
    genotype.MutateDefault(0.);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
      all_default = all_default && it->function_idx == 0;
      all_default = all_default && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    }
    CHECK(all_default);

    // Mutate with 100% probability, everything should have a high chance of changing (input connections will have a 1/2
    // chance of changing (connected vs not connected) while functions will have a 1-1/n chance of changing with n functions)
    genotype.MutateDefault(1.);
    all_default = true;
    for (auto it = genotype.begin(); it != genotype.end(); ++it) {
      all_default = all_default && it->default_output == 0;
      all_default = all_default && it->function_idx == 0;
      all_default = all_default && std::ranges::all_of(it->input_connections, [](char c) { return c == '0'; });
    }
    CHECK_FALSE(all_default);
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
  SECTION("Double") {
    auto rng = std::mt19937(std::random_device{}());
    // Test large range of doubles
    // Use int32 min and max to avoid stoull error with large doubles
    auto min = std::numeric_limits<int32_t>::min();
    auto max = std::numeric_limits<int32_t>::max();
    auto dist = std::uniform_real_distribution<double>(min, max);
    auto fix_double = [](double d) { return std::stod(std::to_string(d)); };
    for (size_t i = 0; i < 100; ++i) {
      auto d = fix_double(dist(rng));
      CHECK(d == base64::B64ToDouble(base64::DoubleToB64(d)));
    }
    // Some specific cases
    CHECK(0 == base64::B64ToDouble(base64::DoubleToB64(0)));
    CHECK(1 == base64::B64ToDouble(base64::DoubleToB64(1)));
    CHECK(-1 == base64::B64ToDouble(base64::DoubleToB64(-1)));
    // Smaller range doubles
    dist = std::uniform_real_distribution<double>(-1, 1);
    for (size_t i = 0; i < 100; ++i) {
      auto d = fix_double(dist(rng));
      CHECK(d == base64::B64ToDouble(base64::DoubleToB64(d)));
    }
  }
}
TEST_CASE("Genotype overloads", "[group7][genotype]") {
  SECTION("operator==") {
    auto genotype = CGPGenotype({7, 2, 0, 10, 3});
    auto genotype2 = CGPGenotype({7, 2, 0, 10, 3});
    CHECK(genotype == genotype2);

    genotype2 = CGPGenotype({7, 2, 0, 10, 2});
    CHECK_FALSE(genotype == genotype2);
    CHECK(genotype != genotype2);

    genotype2 = CGPGenotype({7, 2, 0, 10, 3}).MutateConnections(1);
    CHECK_FALSE(genotype == genotype2);
    CHECK(genotype != genotype2);

    genotype2 = CGPGenotype({7, 2, 0, 10, 3}).MutateFunctions(1, 100);
    CHECK_FALSE(genotype == genotype2);
    CHECK(genotype != genotype2);

    genotype2 = CGPGenotype({7, 2, 0, 10, 3}).MutateOutputs(1, -10000, 10000);
    CHECK_FALSE(genotype == genotype2);
    CHECK(genotype != genotype2);
  }
  SECTION("Copy constructor") {
    auto genotype = CGPGenotype({7, 2, 0, 10, 3}).MutateDefault(1);
    auto genotype2 = CGPGenotype(genotype);
    CHECK(genotype == genotype2);
  }
}
TEST_CASE("Genotype configuration", "[group7][genotype]") {
  SECTION("Exporting and configuration") {
    CGPGenotype genotype({8, 4, 10, 10, 2});
    CGPGenotype genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);

    genotype.begin()->function_idx = 1;
    CHECK_FALSE(genotype == genotype2);
    genotype2.begin()->function_idx = 1;
    CHECK(genotype == genotype2);

    genotype.begin()->input_connections = std::vector<char>(8, '1');
    CHECK_FALSE(genotype == genotype2);
    genotype2.begin()->input_connections = std::vector<char>(8, '1');
    CHECK(genotype == genotype2);

    genotype.begin()->input_connections = std::vector<char>(8, '0');
    CHECK_FALSE(genotype == genotype2);
    genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);

    genotype.begin()->default_output = 1;
    CHECK_FALSE(genotype == genotype2);
    genotype2.begin()->default_output = 1;
    CHECK(genotype == genotype2);

    //
    // These tests could fail, should be unlikely
    //
    genotype.MutateConnections(1);
    CHECK_FALSE(genotype == genotype2);
    genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);

    genotype.MutateFunctions(1, 100);
    CHECK_FALSE(genotype == genotype2);
    genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);

    genotype.MutateOutputs(1, -10000, 10000);
    CHECK_FALSE(genotype == genotype2);
    genotype2 = CGPGenotype().Configure(genotype.Export());
    CHECK(genotype == genotype2);
  }
  SECTION("Copy constructor"){
    CGPGenotype genotype({8, 4, 10, 10, 2});
    CGPGenotype genotype2(genotype);
    CHECK(genotype == genotype2);
  }
}