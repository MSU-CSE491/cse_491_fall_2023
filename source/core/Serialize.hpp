/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Tools to simplify serializing classes.
 * @note Status: ALPHA
 **/


#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace cse491 {

// --- Pre-declarations of Functions ---
template <typename T> static void SerializeValue_Vector(std::ostream &, const std::vector<T> &);
template <typename T> static void SerializeValue_Map(std::ostream &, const T &);
template <typename T> static void DeserializeValue_Vector(std::istream &, std::vector<T> &);
template <typename T> static void DeserializeValue_Map(std::istream &, T &);


// --- Type Traits ---

/// Type trait to determine if we are working with a vector.
template <typename T> struct is_vector : std::false_type {};
template <typename T> struct is_vector<std::vector<T>> : std::true_type {};

/// Type trait to determine if we are working with any type of map.
template <typename T>
struct is_any_map : std::false_type {};
template <typename KEY_T, typename VALUE_T>
struct is_any_map<std::map<KEY_T, VALUE_T>> : std::true_type {};
template <typename KEY_T, typename VALUE_T>
struct is_any_map<std::unordered_map<KEY_T, VALUE_T>> : std::true_type {};

/// Concept to identify if a type can be sent into an ostream.
template <typename STREAM_T, typename OBJ_T>
concept CanStreamTo = requires(STREAM_T & stream, OBJ_T value) {
  { stream << value } -> std::convertible_to<std::ostream&>;
};

/// Concept to identify if a type can be set from an istream.
template <typename STREAM_T, typename OBJ_T>
concept CanStreamFrom = requires(STREAM_T & stream, OBJ_T value) {
  { stream >> value } -> std::convertible_to<std::istream&>;
};

/// Concept to identify if a type has a Serialize() member function.
template <typename OBJ_T>
concept HasSerialize = requires(OBJ_T value) {
  { value.Serialize(std::cout) } -> std::same_as<void>;
};

/// Concept to identify if a type has a Deserialize() member function.
template <typename OBJ_T>
concept HasDeserialize = requires(OBJ_T value) {
  { value.Deserialize(std::cin) } -> std::same_as<void>;
};

/// @brief Helper function to serialize a single member variable.
/// @param os Output stream to write to.
/// @param var Variable to serialize.
template <typename T>
static void SerializeValue(std::ostream & os, const T & var) {
  if constexpr (std::is_enum<T>()) {
    os << static_cast<int>(var) << std::endl;
  } else if constexpr (is_vector<T>()) {
    SerializeValue_Vector(os, var);
  } else if constexpr (is_any_map<T>()) {
    SerializeValue_Map(os, var);
  } else if constexpr (HasSerialize<T>) {
    var.Serialize(os);
  } else if constexpr (CanStreamTo<std::stringstream, T>) {
    os << var << '\n';
  } else {
  }
}

/// @brief Helper specialty function to serialize a vector-based member variable.
/// @param os Output stream to write to.
/// @param var Variable to serialize.
template <typename T>
static void SerializeValue_Vector(std::ostream & os, const std::vector<T> & var) {
  SerializeValue(os, var.size());
  for (const auto & x : var) {
    SerializeValue(os, x);
  }
}

/// @brief Helper specialty function to serialize a unordered_map member variable.
/// @param os Output stream to write to.
/// @param var Variable to serialize.
template <typename T>
static void SerializeValue_Map(std::ostream & os, const T & var) {
  SerializeValue(os, var.size());
  for (const auto & [key, value] : var) {
    SerializeValue(os, key);
    SerializeValue(os, value);
  }
}

/// @brief Helper function to deserialize a single member variable.
/// @param os Input stream to write from.
/// @param var Variable to deserialize.
template <typename T>
static void DeserializeValue(std::istream & is, T & var) {
  static_assert(!std::is_const<T>(), "Cannot deserialize const variables.");

  // If we are loading a string, load it directly.
  if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
    std::getline(is, var, '\n');
  } else if constexpr (is_vector<T>()) {
    DeserializeValue_Vector(is, var);
  } else if constexpr (is_any_map<T>()) {
    DeserializeValue_Map(is, var);
  } else if constexpr (HasDeserialize<T>) {
    var.Deserialize(is);
  } else {
    // @CAO: This can be streamlined to use only the original is, and based on type.
    //       For example, "is << var" followed by "is.peek()" to make sure we have a
    //       newline, and then "is.ignore()" to skip the newline.
    std::string str;
    std::getline(is, str, '\n');
    std::stringstream ss(str);
    if constexpr (std::is_enum<T>()) { // enums must be converted properly.
      int enum_val;
      ss >> enum_val;
      var = static_cast<T>(enum_val);
    } else if constexpr (CanStreamFrom<std::stringstream, T>) {
      ss >> var;
    } else if constexpr (std::is_pointer<T>()) {
      std::cerr << "Warning: Attempting to deserialize pointer." << std::endl;
    } else { 
      // Finally, ignore this value?  Most likely a pointer.
      std::cerr << "Warning: Attempting to deserialize unknown type." << std::endl;
    }
  }
}

/// @brief Helper function to deserialize a member variables from a function
/// @param os Input stream to write from.
/// @param var Variable to serialize.
template <typename T>
static void DeserializeFunction(std::istream & is, std::function<void(T)> set_fun) {
  std::string str;
  std::getline(is, str, '\n');
  if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
    set_fun(str);
  } else if constexpr (std::is_same<std::decay_t<T>, int>()) {
    set_fun(stoi(str));
  } else if constexpr (std::is_same<std::decay_t<T>, double>()) {
    set_fun(stod(str));
  } else {
    T var;
    std::stringstream ss(str);
    ss >> var;
    set_fun(var);
  }
}

/// @brief Helper function to deserialize and return a specified type
/// @param os Input stream to write from.
template <typename T>
static T DeserializeAs(std::istream & is) {
  T value;
  DeserializeValue(is, value);
  return value;
}

/// @brief Helper specialty function to deserialize a vector-based member variable.
/// @param os Input stream to read from.
/// @param var Variable to deserialize.
template <typename T>
static void DeserializeValue_Vector(std::istream & is, std::vector<T> & var) {
  DeserializeFunction<size_t>(is, [&var](size_t in_size){ var.resize(in_size); } );
  for (auto & x : var) {
    DeserializeValue(is, x);
  }
}

/// @brief Helper specialty function to deserialize a unordered_map member variable.
/// @param is Input stream to read from.
/// @param var Variable to deserialize.
template <typename MAP_T>
static void DeserializeValue_Map(std::istream & is, MAP_T & var) {
  size_t map_size = 0;
  typename MAP_T::key_type key;
  typename MAP_T::mapped_type value;
  DeserializeValue(is, map_size);
  for (size_t i = 0; i < map_size; ++i) {
    DeserializeValue(is, key);
    DeserializeValue(is, value);
    var[key] = value;
  }
}

}  // End of namespace cse491
