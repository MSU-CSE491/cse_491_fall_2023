#include "../Group7GP/GPAgent.hpp"
#include "../Group7GP/GPGraph.hpp"

std::vector<size_t> cowboys::EncodeActions(const std::unordered_map<std::string, size_t> &action_map)
{
    std::vector<size_t> actions;
    for (const auto &[action_name, action_id] : action_map)
    {
        actions.push_back(action_id);
    }
    // Sort the actions so that they are in a consistent order.
    std::sort(actions.begin(), actions.end());
    return actions;
}

std::vector<double> cowboys::EncodeState(const cse491::WorldGrid &grid,
                                         const cse491::type_options_t & /*type_options*/,
                                         const cse491::item_set_t & /*item_set*/,
                                         const cse491::agent_set_t & /*agent_set*/,
                                         const cowboys::GPAgent *agent)
{

    /// TODO: Implement this function properly.
    std::vector<double> inputs;

    auto current_position = agent->GetPosition();

    double current_state = grid.At(current_position);
    double above_state = grid.IsValid(current_position.Above()) ? grid.At(current_position.Above()) : 0.;
    double below_state = grid.IsValid(current_position.Below()) ? grid.At(current_position.Below()) : 0.;
    double left_state = grid.IsValid(current_position.ToLeft()) ? grid.At(current_position.ToLeft()) : 0.;
    double right_state = grid.IsValid(current_position.ToRight()) ? grid.At(current_position.ToRight()) : 0.;

    auto extra_state = agent->GetExtraState();
    double prev_action = extra_state["previous_action"];

    inputs.insert(inputs.end(), {prev_action, current_state, above_state, below_state, left_state, right_state});

    return inputs;
}