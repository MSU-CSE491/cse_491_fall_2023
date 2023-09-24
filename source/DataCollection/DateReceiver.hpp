
#include <iostream>
#include "core/Entity.hpp"
#include <vector>

namespace GroupTwo{

    class  cse491::Entity;

    class DataReceiver {

    public:
        
        void DisplayPosition() {
            
            storage.push_back(positions);
          
        }

    private:
        std::vector<cse491::GridPosition> storage;
        cse491::GridPosition positions;
        cse491::Entity x;
    };
}
