//
//  demomission.hpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 27/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef demo_mission_hpp
#define demo_mission_hpp

#include "state_machine.hpp"
#include "../common/service_provider.hpp"
#include "thread_locks.hpp"
#include <thread>
#include <iostream>
#include <boost/python/object_core.hpp>
#include <boost/python/import.hpp>

namespace VehicleModule {
    namespace Mission {
        class DemoMission : public StateMachine {
        public:
            DemoMission(double lat, double lon, double alt);
            
        private:
            double  _lat, _lon, _alt;
            
            bool    _takeoff();
            bool    _goto_position();
            bool    _land();
            bool    _should_transit() const;
        };
    }
}

#endif /* demo_mission_hpp */
