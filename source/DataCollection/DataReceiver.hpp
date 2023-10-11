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
    protected:
    std::vector<T> storage;
    };
}
