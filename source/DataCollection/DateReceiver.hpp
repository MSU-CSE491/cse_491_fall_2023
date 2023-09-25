#include <vector>
#include <core/Entity.hpp>

namespace GroupTwo{

    class Entity;

    class DataReceiver {

    public:
    std::vector<cse491::GridPosition> storage;
        
        DataReceiver(const cse491::Entity& entity) {
      storage.push_back(entity.GetPosition());
    }

        friend std::ostream& operator<<(std::ostream& os, const DataReceiver& obj) {
        os << "MyClass data: " << obj.storage[0].GetX();
        return os;
    }

    protected:

    };
}
