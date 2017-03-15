#include "state_machine.hpp"

#define DUMMY_LAST_STATE_ID -1

using namespace VehicleModule::Mission;
using namespace std;

StateMachine::StateMachine() : _current_state(0) {
    State dummyLastState(DUMMY_LAST_STATE_ID, "Dummy last state", NULL);
    _states[DUMMY_LAST_STATE_ID] = dummyLastState;
}

void StateMachine::start() {
    ::Common::GilLock lk;
    lk.unlock();
    if (_states.find(STARTING_STATE_ID) == _states.end()) {
        throw StateMachineException("No starting state");
    }
    std::cout<< "mark3";
    int last_id = DUMMY_LAST_STATE_ID;
    bool machine_may_stop = false;
    for (unordered_map<int, State>::const_iterator itr = _states.cbegin(); itr != _states.cend(); ++itr) {
        if (any_of(itr->second.transitions.cbegin(), itr->second.transitions.cend(), [&last_id](const Transition& transition) { return transition.dest == last_id; })) {
            machine_may_stop = true;
        }
    }

    if (!machine_may_stop) {
        throw StateMachineException("State machine doesn't stop");
    }

    Common::Logger::info("Starting mission", TAG);
    while (_current_state != DUMMY_LAST_STATE_ID) {
        loop();
    }

    Common::Logger::info("Mission completed", TAG);
    lk.lock();
}

void StateMachine::_add_state(const State& state) {
    if (_states.find(state.id) != _states.end()) {
        throw StateMachineException("State" + to_string(state.id) + " already exists");
    }

    _states[state.id] = state;
}

void StateMachine::_add_transition(const Transition& transition) {
    if (_states.find(transition.origin) == _states.end()
        || _states.find(transition.dest) == _states.end()) {
        throw StateMachineException("State doesn't exist");
    }
    _states[transition.origin].transitions.push_back(transition);
}

int StateMachine::loop() {
    const State& curr = _get_current_state();
    Common::Logger::state(curr.id, curr.description, TAG);

    bool state_result = true;
    if (curr.function) {
        state_result = (this->*curr.function)();
    }
    else {
        Common::Logger::warn("No function for state " + to_string(curr.id), TAG);
    }
    _last_state_succeed = state_result;

    for (vector<Transition>::const_iterator itr = curr.transitions.cbegin(); itr != curr.transitions.cend(); ++itr) {
        if ((this->*itr->function)()) {
            _current_state = itr->dest;
        }
    }

    return state_result;
}

const StateMachine::State& StateMachine::_get_current_state() {
    return _states[_current_state];
}

bool StateMachine::_transit_to_last() const {
    return _last_state_succeed;
}

StateMachine::State::State(int id, const string& description, st_func function, bool last)
: id(id), description(description), function(function), last(last) {
    if (last) {
        transitions.push_back(Transition(id, DUMMY_LAST_STATE_ID, &StateMachine:: _transit_to_last));
    }
}

StateMachine::Transition::Transition(int origin, int dest, tr_func function)
: origin(origin), dest(dest), function(function) {
    if (!function) {
        throw StateMachineException("Transition must have a function. Got null");
    }
}
