/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate the language system.
 * @note Status:TESTING
 **/

// Include the modules that we will be using.
#include "Worlds/Language.hpp"
#include "Worlds/ProgramExecutor.hpp"
// Used to build the parse tree
#include <tao/pegtl/contrib/parse_tree.hpp>

/*int main()
{
	std::string program = "test()\ntest(1)\nwow=3\na=2+test(123*5+4-6/7,abc)-wow\ntest(123-a,435,7650+8*8)\n";
	
	auto code = worldlang::parse_to_code(program);
	for (auto& c : code){
		std::cout << static_cast<int>(c.type) << ", " << c.value << std::endl;
	}
	
	using worldlang::ProgramExecutor;
	auto test = [](ProgramExecutor& pe){
		//TODO: catch exception here? should never be needed?
		auto args = pe.popArgs();
		auto count = args.size();
		std::cout << "Called test with " << count << " args (note: order reversed here):\n";
		for (size_t i = 0; i < count; ++i){
			auto arg = args[i];
			if (std::holds_alternative<double>(arg)){
				std::cout << std::get<double>(arg);	
			} else if (std::holds_alternative<std::string>(arg)){
				std::cout << std::get<std::string>(arg);
			} else if (std::holds_alternative<ProgramExecutor::Identifier>(arg)){
				std::cout << std::get<ProgramExecutor::Identifier>(arg);
			}
			if (i != count - 1){
				std::cout << ", ";
			} else {
				std::cout << std::endl;
			}
		}
		// result value
		pe.pushStack(0.0);
	};
	
	worldlang::ProgramExecutor pe;
	pe.registerFunction("test", test);
	pe.run(program);
}*/

#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "core/WorldBase.hpp"
#include "Worlds/ProgramExecutor.hpp"

// this is in main so it doesn't affect anything else
using namespace cse491;
using worldlang::ProgramExecutor;

// forward declared for pe
class WorldDerived;

class DerivedExecutor : public ProgramExecutor{
public:
	DerivedExecutor(WorldDerived& world);
};

class WorldDerived : public WorldBase {
	DerivedExecutor pe;
	bool stable = false;
	
protected:
	enum ActionType { REMAIN_STILL = 0, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
	
	/// Provide the agent with movement actions.
	void ConfigAgent(AgentBase &agent) override {
		agent.AddAction("up", MOVE_UP);
		agent.AddAction("down", MOVE_DOWN);
		agent.AddAction("left", MOVE_LEFT);
		agent.AddAction("right", MOVE_RIGHT);
	}
	
public:
	WorldDerived(const std::string& filename) : WorldBase(), pe{*this} {
		if (!(stable = pe.runFile(filename))){
			std::cout << "Error: Failed to run initial script!" << std::endl;
		}
		pe.setVariable("MOVE_UP", static_cast<double>(MOVE_UP));
		pe.setVariable("MOVE_DOWN", static_cast<double>(MOVE_DOWN));
		pe.setVariable("MOVE_LEFT", static_cast<double>(MOVE_LEFT));
		pe.setVariable("MOVE_RIGHT", static_cast<double>(MOVE_RIGHT));
	}
	~WorldDerived() = default;
	
	// Check that loader script ran successfully
	bool isOK(){ return stable; }
	
	/// Allow the agents to perform an action.
	int DoAction(AgentBase &agent, size_t action_id) override {
		// Determine where the agent is trying to move.
		if (agent.HasProperty("DoAction")){
			pe.setVariable("action_id", (double)action_id);
			pe.runFile(agent.GetProperty<std::string>("DoAction"));
		} else {
			GridPosition new_position;
			switch (action_id) {
				case REMAIN_STILL: new_position = agent.GetPosition();
					break;
				case MOVE_UP: new_position = agent.GetPosition().Above();
					break;
				case MOVE_DOWN: new_position = agent.GetPosition().Below();
					break;
				case MOVE_LEFT: new_position = agent.GetPosition().ToLeft();
					break;
				case MOVE_RIGHT: new_position = agent.GetPosition().ToRight();
					break;
			}
			
			// Don't let the agent move off the world or into a wall.
			if (!main_grid.IsValid(new_position)) { return false; }
			if (!IsTraversable(agent, new_position)) { return false; }
			
			// Set the agent to its new postion.
			agent.SetPosition(new_position);
			return true;
		}
	}

	/// Can walk on all tiles except for walls
	// Temp hack until more detail is implemented here
	bool IsTraversable(const AgentBase & /*agent*/, cse491::GridPosition pos) const override {
		return GetCellTypeSymbol(main_grid.At(pos)) != '#';
	}
	
	/// @brief  Add a new type of cell to this world.
	/// @param name A unique name for this cell type
	/// @param desc A longer description of the cell type
	/// @param symbol An (optional) unique symbol for text IO (files, command line)
	/// @return A unique ID associated with this cell type (position in type_options vector)
	size_t AddCellType(const std::string &name, const std::string &desc, char symbol) {
		return WorldBase::AddCellType(name, desc, symbol);
	}
};


DerivedExecutor::DerivedExecutor(WorldDerived& world) : ProgramExecutor(world) {
	// Create a new cell type
	registerFunction("addCellType", [this, &world](ProgramExecutor& pe){
		auto args = pe.popArgs();
		if (args.size() < 3) { error("Wrong number of arguments!"); return; }
		// name, desc, symbol, props (ignored: TODO later)
		auto name = pe.as<std::string>(args[0]);
		auto desc = pe.as<std::string>(args[1]);
		auto symbol = pe.as<std::string>(args[2]);
		std::cout << desc << "," << name << "," << symbol << ",";
		if (!symbol.size()) { error("Symbol cannot be empty!"); return; }
		std::cout << (int)symbol[0] << "\n";
		
		auto id = world.AddCellType(name, desc, symbol[0]);
		
		pe.pushStack(static_cast<double>(id));
	});
}

int main()
{
	WorldDerived world{"../assets/scripts/world_script.ws"};
	if (!world.isOK()){
		return 1;
	}
	
//	world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
	
	// TODO: Determine how to load/store script?
	// TODO: Function definitions... execution...?
	
	world.Run();
}
