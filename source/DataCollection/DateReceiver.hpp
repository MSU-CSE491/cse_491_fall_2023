#include <vector>

namespace GroupTwo {

    class Entity;

    class DataReceiver {

    public:
    DataReceiver() = default;

    inline void store_agent(cse491::GridPosition pos) {
        std::cout << "Adding: (" << pos.GetX() << ", " << pos.GetY() << ")" << std::endl;
        storage.push_back(pos); 
    }

    private:
    std::vector<cse491::GridPosition> storage;

    };
}
