/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief A temporary item class to refer to, will be deleted and replaced later
 * @note Status: PROPOSAL
 * @author Yousif Murrani
 **/

#pragma once

#include <string>

class Item {
private:
	std::string name;
	std::string type;
	int value;

public:
	// Constructor
	Item(const std::string& name, const std::string& type, int value)
		: name(name), type(type), value(value) {}

	// Getters
	const std::string& GetName() const { return name; }
	const std::string& GetType() const { return type; }
	int GetValue() const { return value; }
};