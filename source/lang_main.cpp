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
		pe.setVariable("MOVE_UP", static_cast<double>(MOVE_UP));
		pe.setVariable("MOVE_DOWN", static_cast<double>(MOVE_DOWN));
		pe.setVariable("MOVE_LEFT", static_cast<double>(MOVE_LEFT));
		pe.setVariable("MOVE_RIGHT", static_cast<double>(MOVE_RIGHT));
		
		if (!(stable = pe.runFile(filename))){
			std::cout << "Error: Failed to run initial script!" << std::endl;
		}
	}
	~WorldDerived() = default;
	
	// Check that loader script ran successfully
	bool isOK(){ return stable; }
	
	/// Allow the agents to perform an action.
	int DoAction(AgentBase &agent, size_t action_id) override {
		// Determine where the agent is trying to move.
		if (agent.HasProperty("DoAction")){
			pe.setVariable("agent", agent.GetID());
			pe.setVariable("action_id", (double)action_id);
			pe.runFile(agent.GetProperty<std::string>("DoAction"));
			return 1;
		}
		return 0;
	}

	/// Can walk on all tiles except for walls and water (unless agent has property set)
	bool IsTraversable(const AgentBase & agent, cse491::GridPosition pos) const override {
		if (GetCellTypes().at(main_grid.At(pos)).HasProperty(CellType::CELL_WALL))
			return false;
		else if (GetCellTypes().at(main_grid.At(pos)).HasProperty(CellType::CELL_WATER))
			return agent.HasProperty("Swimmer");
		else
			return true;
	}
	
	/// @brief  Add a new type of cell to this world.
	/// @param name A unique name for this cell type
	/// @param desc A longer description of the cell type
	/// @param symbol An (optional) unique symbol for text IO (files, command line)
	/// @return A unique ID associated with this cell type (position in type_options vector)
	size_t AddCellType(const std::string &name, const std::string &desc, char symbol) {
		return WorldBase::AddCellType(name, desc, symbol);
	}
	
	void SetCellProperty(size_t	id, std::string prop){
		type_options[id].SetProperty(prop);
	}
};

DerivedExecutor::DerivedExecutor(WorldDerived& world) : ProgramExecutor(world) {
	// Check if location is valid (on current grid of this world)
	registerFunction("isValid", [this, &world](ProgramExecutor& pe){
		auto args = pe.popArgs();
		if (args.size() != 2) { error("Wrong number of arguments!"); return; }
		
		auto x = pe.as<double>(args[0]);
		auto y = pe.as<double>(args[1]);
		
		pe.pushStack(static_cast<double>(world.GetGrid().IsValid(x,y)));
	});
	// Check if tile is traversable (for this agent)
	registerFunction("isTraversable", [this, &world](ProgramExecutor& pe){
		auto args = pe.popArgs();
		if (args.size() != 3) { error("Wrong number of arguments!"); return; }
		
		auto agent_id = pe.as<size_t>(args[0]);
		auto x = pe.as<double>(args[1]);
		auto y = pe.as<double>(args[2]);
		
		pe.pushStack(static_cast<double>(world.IsTraversable(world.GetAgent(agent_id), {x,y})));
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
