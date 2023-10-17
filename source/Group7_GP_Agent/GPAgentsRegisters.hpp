
#pragma once

// #include <array>
#include <assert.h>

#include <iostream>
#include <vector>

namespace cowboys {

/**
 * @brief Class to hold the registers of the agent
 *
 */
class GPAgentRegisters {
 private:
  /// Vector of registers
  std::vector<size_t> registers;

  /// Number of registers Default is 16
  int numRegisters = 16;

 public:
  /**
   * Constructor for GPAgentRegisters
   * @param numRegisters
   */
  GPAgentRegisters(int numRegisters = 16) {
    registers.resize(numRegisters);
    this->numRegisters = numRegisters;
  }

  /**
   * Destructor for GPAgentRegisters
   */
  ~GPAgentRegisters() = default;

  /**
   * @brief Set the Register object
   * @param index
   * @param value
   * @return
   */
  bool setRegister(size_t index, size_t value) {
    if (index >= registers.size()) {
      //                assert(index >= registers.size());
      return false;
    }
    registers[index] = value;
    return true;
  }

  /**
   * @brief Get the Register object
   * @param index
   * @return
   */
  size_t getRegister(size_t index) {
    if (index >= registers.size()) {
      //                assert(index >= registers.size());
      return 0;
    }
    return registers[index];
  }

  /**
   * @brief Get the number of registers
   * @return
   */
  int getNumRegisters() { return numRegisters; }

  //        friend std::ostream &operator<<(std::ostream &os, GPAgentRegisters
  //        registers) {
  //            os << "registers: ";
  //            for (int i = 0; i < registers.getNumRegisters(); ++i) {
  //                os << registers.getRegister(i) << " ";
  //            }
  //            return os;
  //        }

  /**
   * @brief Iterator class for GPAgentRegisters
   *
   */
  class iterator {
   private:
    /// Index of the iterator
    size_t index;

    /// Reference to the registers
    GPAgentRegisters &registers;

   public:
    /**
     * Constructor for iterator
     * @param index
     * @param regs
     */
    iterator(size_t index, GPAgentRegisters &regs)
        : index(index), registers(regs) {}

    /**
     * Overload dereference operator
     * @return
     */
    size_t &operator*() { return registers.registers[index]; }

    /**
     * Overload pre-increment operator
     * @return
     */
    iterator &operator++() {
      ++index;
      return *this;
    }

    /**
     * @brief Overload post-increment operator
     * @param other
     * @return
     */
    bool operator==(const iterator &other) const {
      return index == other.index;
    }

    /**
     * @brief Overload not equal operator
     * @param other
     * @return
     */
    bool operator!=(const iterator &other) const {
      return index != other.index;
    }
  };

  // Begin iterator
  iterator begin() { return iterator(0, *this); }

  // End iterator
  iterator end() { return iterator(numRegisters, *this); }
};

}  // namespace cowboys