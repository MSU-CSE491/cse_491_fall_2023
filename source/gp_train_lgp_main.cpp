
#include "Agents/GP/GPTrainingLoop.hpp"
#include "Agents/GP/LGPAgent.hpp"
#include "Worlds/MazeWorld.hpp"

#include "Interfaces/TrashInterface.hpp"
#include "Worlds/ManualWorld.hpp"


#include <thread>


#include <vector>

int main() {

//  const int num_threads = std::thread::hardware_concurrency();
    const int num_threads = 6;
  std::cout << "Number of threads: " << num_threads << std::endl;


  auto start_time = std::chrono::high_resolution_clock::now();

  cowboys::GPTrainingLoop <cowboys::LGPAgent, cse491::MazeWorld> loop(false);


  loop.Initialize(50, 5000);
  loop.Run(150, 70, num_threads, false);


  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  auto seconds = duration.count() / 1000000.0;
  std::cout << "Time taken by Training: " << seconds << " seconds" << std::endl;

  return  0;
}