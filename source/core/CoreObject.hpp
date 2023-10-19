/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A common interface class for core objects that sets up required functionality.
 * @note Status: PROPOSAL
 * 
 * This CoreObject class builds an interface for all of the core object, ensuring that
 * they can be properly serialized.
 * 
 * Derived classes must implement:
 *  GetTypeName_impl() - To return the qualified type name.
 *  Serialize_impl() - To store the object in a stream.
 *  Deserialize_impl() - To restore the object from a stream.
 **/

/**
 * @author @amantham20
 * uses as attibute to mark experimental classes and functions
 */
#ifndef NDEBUG
#define EXPERIMENTAL_CLASS            \
  __attribute__((                     \
      annotate("experimental_class"), \
      warning(                        \
          "This is an experimental class and should be used with caution.")))
#define EXPERIMENTAL_FUNCTION                                              \
  __attribute__((annotate("experimental_function"),                        \
                 warning("This is an experimental function and should be " \
                         "used with caution.")))

#else

#define EXPERIMENTAL_CLASS                                                     \
  __attribute__((annotate("experimental_class"),                               \
                 error("This is an experimental class and is currently being " \
                       "worked on and should not be used in release")))
#define EXPERIMENTAL_FUNCTION                                               \
  __attribute__((annotate("experimental_function"),                         \
                 error("This is an experimental function and is currently " \
                       "being worked on and should not be used in release")))
#endif

#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace cse491 {

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
    /// @param os Output stream to serialize into.
    void StartSerialize(std::ostream & os) const {
      os << ":::START " << GetTypeName() << "\n";
    }

    /// @brief Set up end of the serialization for this class (allows checking later)
    /// @param os Output stream to serialize into.
    void EndSerialize(std::ostream & os) const {
      os << ":::END " << GetTypeName() << "\n";
    }

    /// @brief Set up beginning of the deserialization for this class (does type checking)
    /// @param is Input stream to deserialize from.
    /// @return success (true/false)
    bool StartDeserialize(std::istream & is) {
      std::string line;
      std::getline(is, line);
      std::string expected = ":::START " + GetTypeName();
      if (line != expected) {
        std::cerr << "Deserialization error.  Expected: " << expected
                  << "...Found: " << line;
        return false;
      }
      return true;
    }

    /// @brief Set up end of the deserialization for this class (does type checking)
    /// @param is Input stream to deserialize from.
    /// @return success (true/false)
    bool EndDeserialize(std::istream & is) {
      std::string line;
      std::getline(is, line);
      std::string expected = ":::END " + GetTypeName();
      if (line != expected) {
        std::cerr << "Deserialization error.  Expected: " << expected
                  << "...Found: " << line;
        return false;
      }
      return true;
    }

  public:
    virtual ~CoreObject() { }

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
    void Serialize(std::ostream & os) const {
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
    void Deserialize(std::istream & is) {
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

} // End of namespace cse491