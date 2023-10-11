#pragma once

#include <vector>
namespace DataCollection {

    template <typename T>
    class DataReceiverTest {

    public:
    DataReceiverTest() = default;
    virtual ~DataReceiverTest() = default;
    inline void store_data(T obj) { storage.push_back(obj); }
    std::vector<T>& getStorage() {
        return storage;
    }
    inline void store_actions(T obj){
        actions.push_back(obj);
    }
    inline auto &getActions(){
        return actions;
    }

    protected:
    std::vector<T> storage;
    std::vector<T> actions;

    };
}
