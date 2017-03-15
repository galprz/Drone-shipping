//
//  up_down_mission.hpp
//  vehicle
//
//  Created by Gal Malka on 05/01/2017.
//  Copyright © 2017 Gal Malka. All rights reserved.
//

#ifndef up_down_mission_hpp
#define up_down_mission_hpp

#include "state_machine.hpp"
#include "../common/service_provider.hpp"
#include "thread_locks.hpp"
#include <thread>
#include <iostream>
#include <boost/python/object_core.hpp>
#include <boost/python/import.hpp>

namespace VehicleModule {
	namespace Mission {
		class UpDownMission : public StateMachine {
		public:
			UpDownMission(double altitude);
		private:
			double _altitude;
			
			bool    _takeoff();
			bool    _land();
			bool    _should_transit() const;
		};
	}
}

#endif /* up_down_mission_hpp */
