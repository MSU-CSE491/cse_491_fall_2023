#pragma once

#include <vector>

namespace DataCollection {

    struct DataStorage
    {
        /* data */
        std::vector<std::unordered_map<std::string, size_t>> actions;

    };
    
    template <typename T>
    class DataReceiver {

    public:
        DataReceiver() = default;
        virtual ~DataReceiver() = default;
        virtual void DebugPrint() = 0;
        inline void store_data(T obj) { storage.push_back(obj); }
        inline void store_actions(std::unordered_map<std::string, size_t> action){
            DataStorage DataStorage;
            DataStorage.actions.push_back(action);
        }
    protected:
        std::vector<T> storage;
    };
}
