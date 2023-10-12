#pragma once

#include <vector>
namespace DataCollection {

    /**
     * @brief Generic data receiver template class.
     *
     * This class provides a template for storing and retrieving data objects of a specified type.
     * @tparam T The type of data to be stored.
     */
    template <typename T>
    class DataReceiver {
    public:
        /**
         * @brief Default constructor for DataReceiver class.
         */
        DataReceiver() = default;

        /**
         * @brief Virtual destructor for DataReceiver class.
         */
        virtual ~DataReceiver() = default;

        /**
         * @brief Stores a data object of type T.
         * @param obj The data object to be stored.
         */
        inline void store_data(T obj) {
            storage.push_back(obj);
        }

        /**
         * @brief Retrieves the stored data objects.
         * @return Reference to the vector containing stored data objects.
         */
        std::vector<T>& getStorage() {
            return storage;
        }

    protected:
        std::vector<T> storage; ///< Vector to store data objects of type T.
    };
} // namespace DataCollection
