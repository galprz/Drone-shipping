
#ifndef analyze_image_mission_hpp
#define analyze_image_mission_hpp

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
#include "../algorithm/image_algorithm.hpp"
#include <chrono>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

namespace VehicleModule {
    namespace Mission {
        class AnalyzeImageMission : public StateMachine {
        public:
            AnalyzeImageMission();
        private:

            bool    _analyzing_image();

        };
    }
}

#endif /* coarse_scan_mission_hpp */
