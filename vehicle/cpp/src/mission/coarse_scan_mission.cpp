//
//  coarse_scan_mission.cpp
//  vehicle
//
//  Created by Gal Malka on 31/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "coarse_scan_mission.hpp"

using namespace VehicleModule::Mission;
using namespace VehicleModule::Video;
using namespace VehicleModule::Algorithm;
using namespace boost;

CoarseScanMission::CoarseScanMission(double altitude, double distance, double number_of_moves)
: _altitude(altitude), _distance(distance), _number_of_moves(number_of_moves) {
    _add_state(State(0, "Takeoff", static_cast<StateMachine::st_func>(&CoarseScanMission::_takeoff)));
    _add_state(State(1, "Coarse Scan", static_cast<StateMachine::st_func>(&CoarseScanMission::_scan)));
    _add_state(State(2, "Land", static_cast<StateMachine::st_func>(&CoarseScanMission::_land), true));
    _add_transition(Transition(0, 1, static_cast<StateMachine::tr_func>(&CoarseScanMission::_should_transit)));
    _add_transition(Transition(1, 2, static_cast<StateMachine::tr_func>(&CoarseScanMission::_should_transit)));
}

bool CoarseScanMission::_takeoff() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "takeoff", _altitude);
    END_PYTHON_EXECUTION
    return ret;
}

bool CoarseScanMission::_scan() {
    VideoProvider& video_provider = VideoProvider::get_instance();
    Modifiers::Collection collection;
    std::shared_ptr<Modifiers::Mask::TargetCenter> target_center_mask(new Modifiers::Mask::TargetCenter());
    collection.add_modifier(std::shared_ptr<Modifiers::AbstractModifier>(target_center_mask));
    VideoProvider::get_instance().set_channel(VideoProvider::Channel::DEBUG, collection);

    // TODO: calculate correct distance
    int moves_left_in_direction = 1;
    int num_moves_in_direction = 1;
    int j = 0, x = 0, y = _distance, sign = 1;
    while (j < _number_of_moves) {
        int target_counter = 0;
        for (int i=0; i < NUM_IMAGE_TO_SCAN; i++) {
            cv::Mat frame;
            cv::Point target_center;
            long long image_ts;
            if (video_provider.get_frame(frame, VideoProvider::Channel::DEFAULT, image_ts)) {
                if (find_bullseye(frame, target_center)) {
                    target_counter++;
                    target_center_mask->set_target_center(target_center);
                    Common::Logger::debug("Found target center x:" + std::to_string(target_center.x) + " y:" + std::to_string(target_center.y) ,COARSE_SCAN_MISSION);
                }
            }
            if (target_counter >= TARGET_THRESHOLD) {
                VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
                return true;
            }
            if (!video_provider.new_frame_exist(image_ts)) {
                video_provider.wait_to_next_frame();
            }

        }

        if (moves_left_in_direction-- == 0) {
            num_moves_in_direction += 1 - (j % 2);
            moves_left_in_direction = num_moves_in_direction;
            sign = j++ % 2 == 0 ? sign : -sign;
            int tmp = x;
            x = abs(y) * sign;
            y = abs(tmp) * sign;
        }

        BEGIN_PYTHON_EXECUTION
        PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
        python::call_method<void>(vehicle_control, "goto_xy", x, y);
        END_PYTHON_EXECUTION
    }

    VideoProvider::get_instance().clear_channel(VideoProvider::Channel::DEBUG);
    return false;
}

bool CoarseScanMission::_land() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "land");
    END_PYTHON_EXECUTION
    return ret;
}

bool CoarseScanMission::_should_transit() const {
    return _last_state_succeed;
}
