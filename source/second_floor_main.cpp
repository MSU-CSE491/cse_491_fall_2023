#include "Interfaces/TrashInterface.hpp"
#include "Worlds/SecondWorld.hpp"

int main() {
    group4::SecondWorld world;
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    world.Run();

    return 0;
}