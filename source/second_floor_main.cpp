#include "Interfaces/TrashInterface.hpp"
#include "Worlds/SecondFloor.hpp"

int main() {
    group4::SecondFloor world;
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    world.Run();

    return 0;
}