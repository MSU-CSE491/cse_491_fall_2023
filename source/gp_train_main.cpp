
#include "Agents/GP/GPTrainingLoop.hpp"
#include "Agents/GP/LGPAgent.hpp"
#include "Worlds/MazeWorld.hpp"

#include "Interfaces/TrashInterface.hpp"
#include "Worlds/ManualWorld.hpp"
#include "Worlds/GenerativeWorld.hpp"

#include <thread>


#include <vector>

int main() {

    const int num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << num_threads << std::endl;


    auto start_time = std::chrono::high_resolution_clock::now();
//    for (size_t i = 0; i < 10; ++i){


    cowboys::GPTrainingLoop <cowboys::CGPAgent, group6::GenerativeWorld> loop;


    loop.initialize(12, 20);
    loop.run(100, 100, num_threads);
//    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    auto seconds = duration.count() / 1000000.0;
    std::cout << "Time taken by function: " << seconds << " seconds" << std::endl;



    return  0;
}