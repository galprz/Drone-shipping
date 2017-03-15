//
//  coarse_scan_mission.hpp
//  vehicle
//
//  Created by Gal Malka on 31/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef find_and_land_mission_hpp
#define find_and_land_mission_hpp

#include "state_machine.hpp"
#include "../common/service_provider.hpp"
#include "../common/logger.hpp"
#include "thread_locks.hpp"
#include <thread>
#include <iostream>
#include <boost/python/object_core.hpp>
#include <boost/python/import.hpp>
#include "../video/modifiers/mask/target_center.hpp"
#include "../video/modifiers/mask/direction_vector.hpp"
#include "../video/modifiers/collection.hpp"
#include "../video/video_provider.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include "../algorithm/image_algorithm.hpp"
#include<math.h>
#include <opencv2/core/types.hpp>
#include <string>

#define NUM_IMAGE_TO_SCAN 30
#define TARGET_THRESHOLD 3
#define FIND_AND_LAND_TAG "FindAndLandMission"

namespace VehicleModule {
	namespace Mission {
		class FindAndLandMission : public StateMachine {
		public:
			FindAndLandMission(double altitude, double distance, double number_of_moves);
		private:
			double _altitude, _distance, _number_of_moves;
			bool    _takeoff();
			bool    _scan();
			bool    _land();
            bool    _fine_scan();
			bool    _should_transit() const;

            //helpers
            bool _try_get_accurate_altitude(double & out);
		};
	}
}

#endif /* coarse_scan_mission_hpp */
