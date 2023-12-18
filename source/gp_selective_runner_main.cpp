/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

#include "Agents/GP/GPAgentBase.hpp"
#include "Agents/GP/LGPAgent.hpp"
#include "Agents/GP/CGPAgent.hpp"

#include <unistd.h>


void Printgrid(const cse491::MazeWorld &mazeWorld, char symbol = 'S', std::vector<cse491::AgentBase*> agents = {}) {


  size_t arena = 0;
  auto &grid = mazeWorld.GetGrid(arena);
  std::vector<std::string> symbol_grid(grid.GetHeight());





  const auto &type_options = mazeWorld.GetCellTypes();
  // Load the world into the symbol_grid;
  for (size_t y = 0; y < grid.GetHeight(); ++y) {
    symbol_grid[y].resize(grid.GetWidth());
    for (size_t x = 0; x < grid.GetWidth(); ++x) {
      symbol_grid[y][x] = type_options[grid.At(x, y)].symbol;
    }
  }


  // Load the agents into the symbol_grid;
  for (auto agent : agents) {
    auto position = agent->GetPosition();
    symbol_grid[position.GetY()][position.GetX()] = symbol;
  }

  std::cout << "    ";
  for (size_t x = 0; x < grid.GetWidth(); ++x) {
    if (x % 10 == 0 && x != 0) {
      std::cout << x / 10; // Print the ten's place of the column number
    } else {
      std::cout << " "; // Space for non-marker columns
    }
  }
  std::cout << "\n";

  // Print column numbers
  std::cout << "    "; // Space for row numbers
  for (size_t x = 0; x < grid.GetWidth(); ++x) {
    std::cout << x % 10; // Print only the last digit of the column number
  }
  std::cout << "\n";

  // Print out the symbol_grid with a box around it.
  std::cout << "   +" << std::string(grid.GetWidth(), '-') << "+\n";
  for (size_t y = 0; y < grid.GetHeight(); ++y) {

    if (y % 10 == 0 && y != 0) {
      std::cout << y / 10 << " "; // Print the ten's place of the row number
    } else {
      std::cout << "  "; // Space for non-marker rows
    }

    // Print row number
    std::cout << y % 10 << "|"; // Print only the last digit of the row number
    for (char cell: symbol_grid[y]) {
      std::cout << cell;
    }
    std::cout << "|\n";
  }

  std::cout << "   +" << std::string(grid.GetWidth(), '-') << "+\n";
  std::cout << std::endl;
}


int main() {
  cse491::MazeWorld world;
/**
   <CGPAgent>
            <genotype fitness="15.452909001539544" seed="10">6,4,3,9,6;0.j.606uA3:0.0.61duDu:0.3.600:0.0.60iLO:0.5.602j4n:0.n.603vSU:0.6.614Prx:0.g.61xFL:0.l.602iIM:0.h.60185j:00w.3.619bwS:40g.n.619V/2:0.3.611Z3a:2w0.k.601p/w:40g.l.61iZK:0.m.602U29:0.0.601RtI:1g0.8.603nKR:0.4.604iMQ:0.m.61g6tL:0.j.602df1:a8j8.3.61bYRZ:0.f.612vfs:a084.3.6114Xe:0A60.e.614pTw:0.c.613CKA:0.j.611NQG:0.3.60765e:000e1w.f.602oxB:00085g.l.601xVk:000i1c.m.600:</genotype>
        </CGPAgent>
 */

  //GP agent
  auto& GPagent = static_cast<cowboys::GPAgentBase &>(world.AddAgent<cowboys::CGPAgent>("GP 1").SetProperty("symbol", 'G'));
  GPagent.Import("6,4,3,9,6;0.j.606uA3:0.0.61duDu:0.3.600:0.0.60iLO:0.5.602j4n:0.n.603vSU:0.6.614Prx:0.g.61xFL:0.l.602iIM:0.h.60185j:00w.3.619bwS:40g.n.619V/2:0.3.611Z3a:2w0.k.601p/w:40g.l.61iZK:0.m.602U29:0.0.601RtI:1g0.8.603nKR:0.4.604iMQ:0.m.61g6tL:0.j.602df1:a8j8.3.61bYRZ:0.f.612vfs:a084.3.6114Xe:0A60.e.614pTw:0.c.613CKA:0.j.611NQG:0.3.60765e:000e1w.f.602oxB:00085g.l.601xVk:000i1c.m.600:");
  GPagent.SetPosition(0, 1);
  for (int i = 0; i < 100; ++i) {
    world.RunAgents();

    world.UpdateWorld();
    std::cout << "Generation: " << i << std::endl;
    Printgrid(world, 'G', {&GPagent});


    constexpr const int kMicrosecondsPerSecond = 170000;
    usleep(kMicrosecondsPerSecond);

  }

}
