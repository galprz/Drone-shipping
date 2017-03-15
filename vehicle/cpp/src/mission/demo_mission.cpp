//
//  demomission.cpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 27/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "demo_mission.hpp"

using namespace VehicleModule::Mission;
using namespace boost;

DemoMission::DemoMission(double lat, double lon, double alt)
: _lat(lat), _lon(lon), _alt(alt) {
    _add_state(State(0, "Takeoff", static_cast<StateMachine::st_func>(&DemoMission::_takeoff)));
    _add_state(State(1, "Go to position", static_cast<StateMachine::st_func>(&DemoMission::_goto_position)));
    _add_state(State(2, "Land", static_cast<StateMachine::st_func>(&DemoMission::_land), true));
    _add_transition(Transition(0, 1, static_cast<StateMachine::tr_func>(&DemoMission::_should_transit)));
    _add_transition(Transition(1, 2, static_cast<StateMachine::tr_func>(&DemoMission::_should_transit)));
}

bool DemoMission::_takeoff() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "takeoff", _alt);
    END_PYTHON_EXECUTION
    return ret;
}

bool DemoMission::_goto_position() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    python::object dronekit = boost::python::import("dronekit");
    python::object location = dronekit.attr("LocationGlobal")(_lat, _lon, _alt);
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "goto_position", location);
    END_PYTHON_EXECUTION
    return ret;
}

bool DemoMission::_land() {
    bool ret;
    BEGIN_PYTHON_EXECUTION
    PyObject* vehicle_control = (PyObject* ) Common::ServiceProvider::get_instance().get_service("vehicle_control");
    ret = python::call_method<bool>(vehicle_control, "land");
    END_PYTHON_EXECUTION
    return ret;
}

bool DemoMission::_should_transit() const {
    return _last_state_succeed;
}
