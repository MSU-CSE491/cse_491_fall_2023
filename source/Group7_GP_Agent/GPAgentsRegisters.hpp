
//#include <array>
#include <vector>
#include <assert.h>
#include <iostream>


namespace cowboys {

    class GPAgentRegisters {
    private:
//    std::array<int, 16> registers;
        std::vector<size_t> registers;

        int numRegisters = 16;


    public:
        GPAgentRegisters(int numRegisters = 16) {
            registers.resize(numRegisters);
            this->numRegisters = numRegisters;
        }

        ~GPAgentRegisters() = default;

        bool setRegister(size_t index, size_t value) {
            if (index >= registers.size()) {
//                assert(index >= registers.size());
                return false;
            }
            registers[index] = value;
            return true;
        }

        size_t getRegister(size_t index) {
            if (index >= registers.size()) {
//                assert(index >= registers.size());
                return 0;
            }
            return registers[index];
        }

        int getNumRegisters() {
            return numRegisters;
        }



//        friend std::ostream &operator<<(std::ostream &os, GPAgentRegisters registers) {
//            os << "registers: ";
//            for (int i = 0; i < registers.getNumRegisters(); ++i) {
//                os << registers.getRegister(i) << " ";
//            }
//            return os;
//        }



        class iterator {
        private:
            size_t index;
            GPAgentRegisters& registers;

        public:
            iterator(size_t index, GPAgentRegisters& regs) : index(index), registers(regs) {}

            // Overload dereference operator
            size_t& operator*() {
                return registers.registers[index];
            }

            // Overload pre-increment operator
            iterator& operator++() {
                ++index;
                return *this;
            }

            // Overload equality operator
            bool operator==(const iterator& other) const {
                return index == other.index;
            }

            // Overload inequality operator
            bool operator!=(const iterator& other) const {
                return index != other.index;
            }
        };

        // Begin iterator
        iterator begin() {
            return iterator(0, *this);
        }

        // End iterator
        iterator end() {
            return iterator(numRegisters, *this);
        }

    };

}