//
//  statemachine.hpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 25/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef state_machine_hpp
#define state_machine_hpp

#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include "../common/vehicle_module_exception.hpp"
#include "../common/logger.hpp"
#include <string>
#include "gil_lock.hpp"

#define STARTING_STATE_ID 0
#define TAG "StateMachine"

using namespace std;

namespace VehicleModule {
    namespace Mission {
        class StateMachine {
        public:
            virtual void start();
            
            struct StateMachineException : public Common::VehicleModuleException {
                StateMachineException(const string& message) : Common::VehicleModuleException(message) {}
            };
            
        protected:
            bool                        _last_state_succeed;
            
            typedef bool (StateMachine::*st_func)();
            typedef bool (StateMachine::*tr_func)() const;
            StateMachine();
            
            class Transition {
            public:
                int     origin, dest;
                tr_func function;
                
                Transition(int origin, int dest, tr_func function);
            };
            
            class State{
            public:
                int                 id;
                string              description;
                bool                last;
                vector<Transition>  transitions;
                st_func             function;
                
                State(int id, const string& description, st_func function, bool last=false);
                State() {}
            };
            
            void _add_state(const State& state);
            void _add_transition(const Transition& transition);
            
        private:
            unordered_map<int, State>   _states;
            int                         _current_state;
            bool                        _transit_to_last() const;
            
            const State& _get_current_state();
            int loop();
        };
    }
}

#endif /* state_machine_hpp */
