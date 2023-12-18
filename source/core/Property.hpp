/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A class to maintain arbitrary data and facilitate its use.
 * @note Status: ALPHA
 **/

#pragma once

#include <cassert>
#include <string>

#include "CoreObject.hpp"
#include "Data.hpp"
#include "GridPosition.hpp"

namespace cse491 {

struct PropertyBase : public CoreObject {
  virtual ~PropertyBase() {}
  virtual PropertyType GetType() const = 0;
  virtual std::string GetTypeName() const = 0;
  virtual std::string ToString() const = 0;
  virtual char ToChar() const = 0;
  virtual double ToDouble() const = 0;
  virtual int ToInt() const = 0;
  virtual GridPosition ToGridPosition() const = 0;
};

// For the moment, properties can be char, int, double, string, or GridPosition
template <typename T>
struct Property : public PropertyBase {
  T value;
  Property(const T &in) : value(in) {}
  Property(T &&in) : value(in) {}

  PropertyType GetType() const override {
    if constexpr (std::is_same<T, char>())         return PropertyType::t_char;
    if constexpr (std::is_same<T, int>())          return PropertyType::t_int;
    if constexpr (std::is_same<T, bool>())         return PropertyType::t_bool;
    if constexpr (std::is_same<T, double>())       return PropertyType::t_double;
    if constexpr (std::is_same<T, std::string>())  return PropertyType::t_string;
    if constexpr (std::is_same<T, GridPosition>()) return PropertyType::t_position;
    return PropertyType::t_other;
  }

  std::string GetTypeName() const override {
    if constexpr (std::is_same<T, char>())         return "char";
    if constexpr (std::is_same<T, int>())          return "int";
    if constexpr (std::is_same<T, bool>())         return "bool";
    if constexpr (std::is_same<T, double>())       return "double";
    if constexpr (std::is_same<T, std::string>())  return "string";
    if constexpr (std::is_same<T, GridPosition>()) return "GridPosition";
    return "unknown";
  }

  std::string ToString() const override {
    if constexpr (std::is_same<T, char>())         return std::string(1, value);
    if constexpr (std::is_same<T, int>())          return std::to_string(value);
    if constexpr (std::is_same<T, double>())       return std::to_string(value);
    if constexpr (std::is_same<T, std::string>())  return value;
    if constexpr (std::is_same<T, GridPosition>()) return value.ToString();
    return "unknown";
  }

  char ToChar() const override {
    if constexpr (std::is_same<T, char>())         return value;
    if constexpr (std::is_same<T, int>())          return static_cast<char>(value);
    if constexpr (std::is_same<T, double>())       return static_cast<char>(value);
    if constexpr (std::is_same<T, std::string>())  return value.size() ? value[0] : '\0';
    if constexpr (std::is_same<T, GridPosition>()) return '\0'; // No conversion.
    return '\0';
  }

  double ToDouble() const override {
    if constexpr (std::is_same<T, char>())         return static_cast<double>(value);
    if constexpr (std::is_same<T, int>())          return static_cast<double>(value);
    if constexpr (std::is_same<T, double>())       return value;
    if constexpr (std::is_same<T, std::string>())  return std::stod(value);
    if constexpr (std::is_same<T, GridPosition>()) return std::nan("nan"); // No conversion.
    return std::nan("nan");
  }

  int ToInt() const override {
    if constexpr (std::is_same<T, char>())         return static_cast<int>(value);
    if constexpr (std::is_same<T, int>())          return value;
    if constexpr (std::is_same<T, double>())       return static_cast<int>(value);
    if constexpr (std::is_same<T, std::string>())  return std::stoi(value);
    if constexpr (std::is_same<T, GridPosition>()) return 0; // No conversion.
    return 0;
  }

  GridPosition ToGridPosition() const override {
    if constexpr (std::is_same<T, char>())         return GridPosition::Invalid();
    if constexpr (std::is_same<T, int>())          return GridPosition::Invalid();
    if constexpr (std::is_same<T, double>())       return GridPosition::Invalid();
    if constexpr (std::is_same<T, std::string>())  return GridPosition(value);
    if constexpr (std::is_same<T, GridPosition>()) return value;
    return GridPosition::Invalid();
  }

  // --- CoreObject Functionality ---
  std::string GetTypeName_impl() const override {
    return std::string("cse491::Property<") + GetTypeName() + ">";
  }

  void Serialize_impl(std::ostream & os) const override {
    SerializeValue(os, value);
  };

  void Deserialize_impl(std::istream & is) override {
    DeserializeValue(is, value);
  };
};


}  // End of namespace cse491
