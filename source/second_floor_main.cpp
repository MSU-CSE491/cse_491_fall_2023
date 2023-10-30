#include "Interfaces/TrashInterface.hpp"
#include "Worlds/SecondWorld.hpp"

int main() {
    group4::SecondWorld world("../assets/grids/second_floor.grid", "../assets/second_floor_input.json");
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    world.Run();

    return 0;
}