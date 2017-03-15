//
//  coarse_scan_mission.hpp
//  vehicle
//
//  Created by Gal Malka on 31/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef coarse_scan_mission_hpp
#define coarse_scan_mission_hpp

#include "state_machine.hpp"
#include "../common/service_provider.hpp"
#include "thread_locks.hpp"
#include <thread>
#include <iostream>
#include <boost/python/object_core.hpp>
#include <boost/python/import.hpp>
#include "../video/modifiers/mask/target_center.hpp"
#include "../video/modifiers/collection.hpp"
#include "../video/video_provider.hpp"
#include <opencv2/core/mat.hpp>
#include "../algorithm/image_algorithm.hpp"
#include <opencv2/core/types.hpp>

#define NUM_IMAGE_TO_SCAN 30
#define TARGET_THRESHOLD 3
#define COARSE_SCAN_MISSION "CoarseScanMission"

namespace VehicleModule {
	namespace Mission {
		class CoarseScanMission : public StateMachine {
		public:
			CoarseScanMission(double altitude, double distance, double number_of_moves);
		private:
			double _altitude, _distance, _number_of_moves;

			bool    _takeoff();
			bool    _scan();
			bool    _land();
			bool    _should_transit() const;
		};
	}
}

#endif /* coarse_scan_mission_hpp */
