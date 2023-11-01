
#include "Group7_GP_Agent/GPTrainingLoop.hpp"
#include "Group7_GP_Agent/LGPAgent.hpp"
#include "Worlds/MazeWorld.hpp"


#include <vector>

int main() {
//    cse491::MazeWorld world;
//    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
//
//    //GP agent
//    world.AddAgent<cowboys::LGPAgent>("GP 1").SetPosition(1, 0).SetProperty("symbol", 'G');




    cowboys::GPTrainingLoop <cowboys::LGPAgent, cse491::MazeWorld> loop;

    loop.initialize(5, 100);
    loop.run(100, 100);




    return  0;
}