//
//  logger.cpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 25/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "logger.hpp"

using namespace VehicleModule::Common;

void Logger::info(const string& msg, const string& tag) {
    _log(msg, "info", tag);
}

void Logger::warn(const string& msg, const string& tag) {
    _log(msg, "warn", tag);
}

void Logger::critical(const string& msg, const string& tag) {
    _log(msg, "critical", tag);
}

void Logger::debug(const string& msg, const string& tag) {
    _log(msg, "debug", tag);
}

void Logger::state(int id, const string& description, const string& tag) {
    BEGIN_PYTHON_EXECUTION
    PyObject* logger = (PyObject*)ServiceProvider::get_instance().get_service("logger");
    boost::python::call_method<void>(logger, "state", id, description, tag);
    END_PYTHON_EXECUTION
}

void Logger::_log(const string& msg, char const* level, const string& tag) {
    BEGIN_PYTHON_EXECUTION
    PyObject* logger = (PyObject*)ServiceProvider::get_instance().get_service("logger");
    boost::python::call_method<void>(logger, level, msg, tag);
    END_PYTHON_EXECUTION
}
