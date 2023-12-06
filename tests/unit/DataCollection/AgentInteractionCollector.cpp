/**
 * @file AgentInteractionCollector.cpp
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "DataCollection/AgentInteractionCollector.hpp"

TEST_CASE("AgentInteractionCollectorCreate", "[AgentInteractionCollector]")
{
    DataCollection::AgentInteractionCollector data;

    CHECK(data.GetInteractionData().empty());   // Item usage is empty
    CHECK(data.GetUniqueInteractions() == 0);   // No unique interactions
}

TEST_CASE("AgentInteractionCollectorInsert", "[AgentInteractionCollector]")
{
    DataCollection::AgentInteractionCollector _data;
    auto& data = _data.GetInteractionData();

    _data.RecordInteraction("Pacer 1");
    CHECK(_data.GetUniqueInteractions() == 1);
    CHECK(data.at("Pacer 1") == 1);
}

TEST_CASE("AgentInteractionCollectorMultiInsert", "[AgentInteractionCollector]")
{
    DataCollection::AgentInteractionCollector _data;
    auto& data = _data.GetInteractionData();

    _data.RecordInteraction("Pacer 1");
    CHECK(_data.GetUniqueInteractions() == 1);
    CHECK(data.at("Pacer 1") == 1);

    _data.RecordInteraction("Pacer 2");
    CHECK(_data.GetUniqueInteractions() == 2);
    CHECK(data.at("Pacer 2") == 1);

    _data.RecordInteraction("Pacer 1");
    CHECK(_data.GetUniqueInteractions() == 2);
    CHECK(data.at("Pacer 1") == 2);
}
