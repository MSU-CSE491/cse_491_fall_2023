/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Worlds/ManualWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"

void timer(group6::GenerativeWorld& world, int duration) {
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    world.EndGame(false);
}

void runWorld(group6::BiomeType biome, int width, int height, const unsigned int /*SEED*/, int timerDuration = -1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 1000000);
    int random_number = distribution(gen);
    group6::GenerativeWorld world(biome, width, height, random_number);
    world.AddTeleporters();

    if (biome == group6::BiomeType::Grasslands) {
        // Using TrashInterface since MainInterface does not support Grasslands biome
        world.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@').SetName("Player");
    }

    else if (biome == group6::BiomeType::Maze) {
        bool counter = false;
        while (!counter) {
            int random_x = world.GetRandom(18, 53);
            int random_y = world.GetRandom(6, 18);

            if (world.GetGrid().At(random_x, random_y) == 1) {
                world.AddItem("Boots", "symbol", 'B').SetPosition(random_x, random_y).SetName("Boots").SetProperty("Health", 3.0);
                counter = true;
            }
        }
        counter = false;
        while (!counter) {
            int random_x = world.GetRandom(18, 53);
            int random_y = world.GetRandom(6, 18);

            if (world.GetGrid().At(random_x, random_y) == 1) {
                world.AddItem("Shield", "symbol", 'S').SetPosition(random_x, random_y).SetName("Shield").SetProperty("Health", 3.0);
                counter = true;
            }
        }

        world.AddItem("Shield", "symbol", 'S').SetPosition(1, 1).SetName("Shield").SetProperty("Health", 3.0);
        world.AddItem("Boots", "symbol", 'B').SetPosition(2, 2).SetName("Boots").SetProperty("Health", 3.0);
        world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(5, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(8, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 3").SetPosition(10, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 4").SetPosition(12, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 5").SetPosition(15, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 6").SetPosition(18, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 7").SetPosition(20, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 8").SetPosition(22, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 9").SetPosition(25, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 10").SetPosition(28, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 11").SetPosition(30, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 12").SetPosition(32, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 13").SetPosition(35, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 14").SetPosition(38, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 15").SetPosition(40, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 16").SetPosition(42, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 17").SetPosition(45, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 18").SetPosition(48, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 19").SetPosition(50, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 20").SetPosition(52, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 21").SetPosition(55, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 22").SetPosition(58, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 23").SetPosition(60, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 24").SetPosition(62, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 25").SetPosition(65, int(world.GetRandom(0, height-1)));
        world.AddAgent<cse491::PacingAgent>("Pacer 26").SetPosition(70, int(world.GetRandom(0, height-1)));
//        auto & astar_agent = static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar1"));
//        astar_agent.SetPosition(4, 4);
//        astar_agent.SetGoalPosition(1, 1);
//        astar_agent.RecalculatePath();

        world.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@').SetName("Player");

        world.AddArmory();
    }

    else if (biome == group6::BiomeType::Ocean) {
        world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    }

    // Start an optional timer
    if (timerDuration != -1) {
        std::thread timerThread(timer, std::ref(world), timerDuration);
        world.Run();
        timerThread.join();
    }

    else {
        world.Run();
    }
}
int main() {

//    cse491::MazeWorld world;
//    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
//    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
//    world.AddAgent<i_2D::MainInterface>("Interface1").SetProperty<char>("symbol", '@');
//    world.Run();
//
//     group 4 maze -1
//    group4::SecondWorld world_1;
//
//    world_1.AddAgent<i_2D::MainInterface>("Interface", "Health", 5).SetProperty<char>("symbol", '@');
//
//    // Adding power sword with id = 1; name = sword of power
//    auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
//    powerSword->SetProperty("Damage", 20.0);
//    powerSword->SetPosition(1, 2);
//    world_1.AddItem(std::move(powerSword));
//
//    // Adding fire sword with id = 2; name = Inferno Slicer
//    auto infernoSlicer = std::make_unique<cse491::ItemBase>(2, "Inferno Slicer");
//    infernoSlicer->SetProperties("Damage", 12.5, "Speed", 15.0, "Burning Duration", 2.5);
//    infernoSlicer->SetPosition(3, 4);
//
//
//    // Adding a piece of armor with id = 10; name = Daedric
//    auto daedricArmor = std::make_unique<cse491::ItemBase>(10, "Daedric Armor");
//    daedricArmor->SetProperties("Health", 99, "Extra Inv. Space", 5);
//    daedricArmor->SetPosition(5, 0);
//    world_1.AddItem(std::move(daedricArmor));
//    world_1.Run();
//
//    static const unsigned int SEED = 973;
//    group6::BiomeGenerator biomeGenerator(group6::BiomeType::Maze, 110, 25, SEED);
//    biomeGenerator.generate();
//
//    srand(time(NULL));
//    auto path = biomeGenerator.clearPath();
//    biomeGenerator.applyPathToGrid(path);
//    biomeGenerator.saveToFile("../assets/grids/generated_maze.grid");
//
//    group6::GenerativeWorld world_2(group6::BiomeType::Maze, 110, 25, SEED);
//    world_2.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
//    world_2.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
//    world_2.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@');
//
//    world_2.Run();


    auto biome = group6::BiomeType::Maze;          // change world biome type here
    int width = 70;                       // change world width here
    int height = 24;                       // change world height here
    static const unsigned int SEED = 19;   // change world seed here
    int timerDuration = -1;                 // an optional timer length, set to -1 for no timer

    runWorld(biome, width, height, SEED, timerDuration);

    cse491_team8::ManualWorld world_3;
    world_3.AddItem("Axe", "Uses", 5, "symbol", 'P').SetPosition(37, 3);
    world_3.AddItem("Boat", "Uses", 7, "symbol", 'U').SetPosition(18, 4);
    world_3.AddAgent<i_2D::MainInterface>("Interface3", "Strength", 15, "Health", 15, "Max_Health", 40, "Direction",
                                          0).SetProperty("symbol", '@').SetPosition(40, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 30, "Health", 5, "Max_Health", 30, "Direction",
                                          0).SetPosition(45, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 15, "Health", 10, "Max_Health", 30, "Direction",
                                          0).SetPosition(3, 14);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 20, "Health", 20, "Max_Health", 30, "Direction",
                                          0).SetPosition(7, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 25, "Health", 30, "Max_Health", 30, "Direction",
                                          0).SetPosition(18, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 30, "Health", 40, "Max_Health", 30, "Direction",
                                          0).SetPosition(45, 17);

    world_3.GenerateMoveSets();


    world_3.Run();


}
