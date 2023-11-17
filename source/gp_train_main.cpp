
#include "Agents/GP/GPTrainingLoop.hpp"
#include "Agents/GP/LGPAgent.hpp"
#include "Worlds/MazeWorld.hpp"

#include "Agents/GP/CGPAgent.hpp"


#include <thread>


#include <vector>

int main() {
//    cse491::MazeWorld world;
//    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
//
//    //GP agent
//    world.AddAgent<cowboys::LGPAgent>("GP 1").SetPosition(1, 0).SetProperty("symbol", 'G');

#ifdef  __gnu__
    std::cout << "Clang" << std::endl;
#else
    std::cout << "Not Clang" << std::endl;
#endif


    const int num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << num_threads << std::endl;


    auto start_time = std::chrono::high_resolution_clock::now();


    cowboys::GPTrainingLoop <cowboys::LGPAgent, cse491::MazeWorld> loop;

//    loop.initialize(20, 200);
//
//    loop.run(100, 100);

    loop.initialize(1, 20);
    loop.run(10, 100, num_threads);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    auto seconds = duration.count() / 1000000.0;
    std::cout << "Time taken by function: " << seconds << " seconds" << std::endl;



    return  0;
}