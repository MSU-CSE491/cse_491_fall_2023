#include <vector>

namespace GroupTwo{

    class DataReceiver {

    public:
    std::vector<cse491::GridPosition> storage;
    cse491::GridPosition positions;

        
        DataReceiver() {
            storage.push_back(positions);     
        }

        friend std::ostream& operator<<(std::ostream& os, const DataReceiver& obj) {
        os << "MyClass data: " << obj.storage[0].GetX();
        return os;
    }

    protected:

    };
}
