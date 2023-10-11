#pragma once

#include <vector>
namespace DataCollection {

    template <typename T>
    class DataReceiver {

    public:
    DataReceiver() = default;
    virtual ~DataReceiver() = default;
    inline void store_data(T obj) { storage.push_back(obj); }
    std::vector<T>& getStorage() {
        return storage;
    }
    inline void store_actions(std::unordered_map<std::string, size_t> action){
        actions.push_back(action);
    }
    auto &getActions(){
        return actions;
    }

    protected:
    std::vector<T> storage;
    std::vector<std::unordered_map<std::string, size_t>> actions;

    };
}
