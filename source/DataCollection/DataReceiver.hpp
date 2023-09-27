#pragma once

#include <vector>

namespace DataCollection {

    template <typename T>
    class DataReceiver {

    public:
    DataReceiver() = default;
    virtual ~DataReceiver() = default;

    virtual void DebugPrint() = 0;
    inline void store_data(T obj) { storage.push_back(obj); }

    protected:
    std::vector<T> storage;

    };
}
