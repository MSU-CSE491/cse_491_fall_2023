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
         * @brief Retrieves the stored data objects.
         * @return Reference to the vector containing stored data objects.
         */
        std::vector<T>& getStorage() {
            return storage;
        }

        /**
         * @brief Store a data object into the storage
         * @param obj The data object to store
         */
        void StoreIntoStorage(T obj){
            storage.push_back(obj);
        }

        /**
         * @brief Checks if the storage is empty.
         * @return True if the storage is empty, false otherwise.
         */
        bool IsEmpty() {
            return storage.empty();
        }

    protected:
        std::vector<T> storage; ///< Vector to store shared_ptr data objects of type T.
    };
} // namespace DataCollection
