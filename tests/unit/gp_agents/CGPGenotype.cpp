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
/*
TEST_CASE("base64", "[group7][base64]") {
    SECTION("Encode and decode") {
        auto max_ull = std::numeric_limits<size_t>::max();
        std::string max_encoded_ull = "P//////////";
        CHECK(base64::encode(max_ull) == max_encoded_ull);
        CHECK(base64::decode(max_encoded_ull) == max_ull);
        std::string max_ull_str = std::to_string(max_ull);
    }
}
*/
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
    SECTION("Load constructor") {
        std::string encoded_genotype = "8,4,2,10,2";
        CHECK_THROWS(CGPGenotype().Configure(encoded_genotype));

        encoded_genotype = "8,4,2,10;";
        CHECK_THROWS(CGPGenotype().Configure(encoded_genotype));

        encoded_genotype = "8,4,2,10,x;";
        CHECK_THROWS(CGPGenotype().Configure(encoded_genotype));

        encoded_genotype = "8,4,2,10,2;";
        CHECK_NOTHROW(CGPGenotype().Configure(encoded_genotype));

        auto genotype = CGPGenotype().Configure(encoded_genotype);
        genotype.InitGenotype();
        CHECK(genotype.GetNumConnections() == 8 * 10 + (8 + 10) * 10 + (10 + 10) * 4);
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
