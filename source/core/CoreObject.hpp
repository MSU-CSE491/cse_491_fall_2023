/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A common interface class for core objects that sets up required functionality.
 * @note Status: ALPHA
 *
 * This CoreObject class builds an interface for all of the core object, ensuring that
 * they can be properly serialized.
 *
 * Derived classes must implement:
 *  GetTypeName_impl()               - Return the qualified type name.
 *  Serialize_impl(std::ostream &)   - Store the object in a stream.
 *  Deserialize_impl(std::istream &) - Restore the object from a stream.
 **/


#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * @author @amantham20
 * uses as attribute to mark experimental classes and functions
 */
#ifndef NDEBUG
#define EXPERIMENTAL_FUNCTION __attribute__((annotate("experimental_function")))
#define EXPERIMENTAL_CLASS __attribute__((annotate("experimental_class")))

#else // NDEBUG *not* set, so debug mode.
#define EXPERIMENTAL_CLASS                       \
  __attribute__((annotate("experimental_class"), \
                 warning("This is an experimental class and should be used with caution.")))
#define EXPERIMENTAL_FUNCTION                                              \
  __attribute__((annotate("experimental_function"),                        \
                 warning("This is an experimental function and should be " \
                         "used with caution.")))
#endif

namespace cse491 {

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

/// @class WorldGrid
/// @brief A common interface class for core objects that sets up required functionality.
/// This class ensures that objects can be serialized (saved), deserialized (restored)
/// and provides helper functions.
class CoreObject {
protected:
  // -- Required functionality in all core classes --
  // These functions are core functionality that must be implemented in all derived
  // classes.  They will enable the public interface to behave correctly.

  /// @brief  Override to allow access the derived name for this type.
  /// @return A unique typename, ideally the exact name for this class (with scope)
  /// This member function will be helpful for serialization and debugging.  It should
  /// return a fully scoped typename, such as "cse491::WorldGrid".
  virtual std::string GetTypeName_impl() const = 0;

  /// @brief Override to store the current state of this object for later extraction.
  /// @param [in,out] output_stream An ostream that the required information should be stored in.
  virtual void Serialize_impl(std::ostream &) const = 0;

  /// @brief Override to restore the state of this object from the provided stream
  /// @param [in,out] input_stream The istream with full details about this object.
  virtual void Deserialize_impl(std::istream &) = 0;

  // -- Helper functions --
  // The functions below can be used in derived classes to implement above functionality.

  /// @brief Set up beginning of the serialization for this class (allows checking later)
  /// @param os Output stream to Serialize into.
  void StartSerialize(std::ostream &os) const { os << ":::START " << GetTypeName() << "\n"; }

  /// @brief Set up end of the serialization for this class (allows checking later)
  /// @param os Output stream to Serialize into.
  void EndSerialize(std::ostream &os) const { os << ":::END " << GetTypeName() << "\n"; }

  /// @brief Set up beginning of the deserialization for this class (does type checking)
  /// @param is Input stream to deserialize from.
  /// @return success (true/false)
  bool StartDeserialize(std::istream &is) {
    std::string line;
    std::getline(is, line);
    std::string expected = ":::START " + GetTypeName();
    if (line != expected) {
      std::cerr << "Deserialization error.  Expected: " << expected << "...Found: " << line;
      return false;
    }
    return true;
  }

  /// @brief Set up end of the deserialization for this class (does type checking)
  /// @param is Input stream to deserialize from.
  /// @return success (true/false)
  bool EndDeserialize(std::istream &is) {
    std::string line;
    std::getline(is, line);
    std::string expected = ":::END " + GetTypeName();
    if (line != expected) {
      std::cerr << "Deserialization error.  Expected: " << expected << "...Found: " << line;
      return false;
    }
    return true;
  }

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
    } else if constexpr (std::is_base_of<CoreObject, T>()) {
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
    } else if constexpr (std::is_base_of<CoreObject, T>()) {
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
      } else { 
        // Finally, ignore this value?  Most likely a pointer.
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

public:
  virtual ~CoreObject() {}

  auto operator<=>(const CoreObject &) const = default;

  /// @brief  Access the derived name for this type.
  /// @return A unique typename, ideally the exact name for this class (with scope)
  /// This member function will be helpful for serialization and debugging.  It should
  /// return a fully scoped typename, such as "cse491::WorldGrid".
  std::string GetTypeName() const { return GetTypeName_impl(); }

  /// @brief Store the current state of this object for later extraction.
  /// @param [in,out] output_stream An ostream that the required information should be stored in.
  /// A derived class is required to store all information about itself in Serialize()
  /// and clearly note the end of storage to simplify de-serializing the data.
  /// This function will automatic add a start line that reads ":::START" followed by the
  /// typename, and an end line with ":::END" followed by the typename again.
  void Serialize(std::ostream &os) const {
    StartSerialize(os);
    Serialize_impl(os);
    EndSerialize(os);
  }

  /// @brief Restore the state of this object, extracting it from the provided stream
  /// @param [in,out] input_stream The istream with full details about this object.
  /// This function takes in an istream that was previously written using the paired
  /// Serialize() function and restores the associated class.  It should stop loading
  /// when all data is finished; this may be signalled by the ":::END" directive.  The
  /// stream should end ready for the next extraction.
  void Deserialize(std::istream &is) {
    StartDeserialize(is);
    Deserialize_impl(is);
    EndDeserialize(is);
  }

  /// @brief A version of Serialize that accepts a filename for saving just this object
  /// @param [in] filename The name of the file to create for storing this object.
  /// @return Success (true/false)
  bool Serialize(std::string filename) const {
    std::ofstream os(filename);
    if (!os.is_open()) {
      std::cerr << "Could not open file '" << filename << "' for Serialize()." << std::endl;
      return false;
    }
    Serialize(os);
    return true;
  }

  /// @brief A version of Deserialize that accepts a filename to read this object from
  /// @param [in] filename The name of the file to open to load this object from.
  /// @return Success (true/false)
  bool Deserialize(std::string filename) {
    std::ifstream is(filename);
    if (!is.is_open()) {
      std::cerr << "Could not open file '" << filename << "' for Serialize()." << std::endl;
      return false;
    }
    Deserialize(is);
    return true;
  }
};

}  // End of namespace cse491