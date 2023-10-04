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
    inline void store_actions(std::unordered_map<std::string, size_t> actions){
        DataStorage DataStorage;
        DataStorage.actions.push_back(actions);
        }
    protected:
    std::vector<T> storage;

    };
}
