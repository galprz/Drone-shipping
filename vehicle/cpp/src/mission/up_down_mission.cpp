//
//  up_down_mission.cpp
//  vehicle
//
//  Created by Gal Malka on 05/01/2017.
//  Copyright © 2017 Gal Malka. All rights reserved.
//

#include "up_down_mission.hpp"

using namespace VehicleModule::Mission;
using namespace boost;

UpDownMission::UpDownMission(double altitude)
: _altitude(altitude) {
	_add_state(State(0, "Takeoff", static_cast<StateMachine::st_func>(&UpDownMission::_takeoff)));
	_add_state(State(1, "Land", static_cast<StateMachine::st_func>(&UpDownMission::_land), true));
	_add_transition(Transition(0, 1, static_cast<StateMachine::tr_func>(&UpDownMission::_should_transit)));
}

bool UpDownMission::_takeoff() {
	bool ret;
	BEGIN_PYTHON_EXECUTION
	PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
	ret = python::call_method<bool>(vehicle_control, "takeoff", _altitude);
	END_PYTHON_EXECUTION
	return ret;
}

bool UpDownMission::_land() {
	bool ret;
	BEGIN_PYTHON_EXECUTION
	PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
	ret = python::call_method<bool>(vehicle_control, "land");
	END_PYTHON_EXECUTION
	return ret;
}

bool UpDownMission::_should_transit() const {
	return _last_state_succeed;
}
