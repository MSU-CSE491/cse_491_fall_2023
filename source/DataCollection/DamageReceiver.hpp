/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Data receiver class specialized for storing DamageData objects.
 * @note Status: ALPHA
 **/

#pragma once

#include "DamageData.hpp"
#include "DataReceiver.hpp"

namespace DataCollection {

/**
 * @brief Data receiver class specialized for storing DamageData objects.
 *
 * This class extends DataReceiver class and provides specific functionality
 * for storing DamageData objects along with damage sources and amounts.
 */
class DamageReceiver : public DataReceiver<DamageData> {};

}  // namespace DataCollection
