/**
 * @file DamageCollector.cpp
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "DataCollection/DamageCollector.hpp"

TEST_CASE("DamageCollectorCreate", "[DamageCollector]")
{
    DataCollection::DamageCollector damageCollector;
    CHECK(damageCollector.GetDamageAmounts("Sword").empty());
}

TEST_CASE("DamageCollectorInsert", "[DamageCollector]")
{
    DataCollection::DamageCollector damageCollector;

    damageCollector.RecordDamageResult("Sword", 3);
    auto& swordDamage = damageCollector.GetDamageAmounts("Sword");

    CHECK(swordDamage.size() == 1);
    CHECK(damageCollector.CalculateAverageDamage("Sword") == 3.0);

    damageCollector.RecordDamageResult("Sword", 5);
    CHECK(swordDamage.size() == 2);
    CHECK(damageCollector.CalculateAverageDamage("Sword") == 4.0);

    damageCollector.RecordDamageResult("Axe", 9.5);
    auto& axeDamage = damageCollector.GetDamageAmounts("Axe");

    CHECK(axeDamage.size() == 1);
    CHECK(swordDamage.size() == 2);
    CHECK(damageCollector.CalculateAverageDamage("Axe") == 9.5);

    damageCollector.RecordDamageResult("Axe", 6.5);
    CHECK(axeDamage.size() == 2);
    CHECK(damageCollector.CalculateAverageDamage("Axe") == 8.0);
}
