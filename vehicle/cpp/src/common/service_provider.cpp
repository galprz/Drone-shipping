//
//  serviceprovider.cpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 28/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "service_provider.hpp"

using namespace VehicleModule::Common;

void ServiceProvider::publish(const string& name, void* service) {
    _read_write_lock.read_lock();
    bool found_service = _services.find(name) != _services.end();
    _read_write_lock.read_unlock();

    if (found_service) {
        throw ServiceProviderException("Service with name '" + name + "' already exists");
    }

    _read_write_lock.write_lock();
    _services[name] = service;
    _read_write_lock.write_unlock();
}

void ServiceProvider::publish(const string& name, PyObject* service) {
    publish(name,(void *)service);
}

void* ServiceProvider::get_service(const string& name) {
    _read_write_lock.read_lock();
    if (_services.find(name) == _services.end()) {
        _read_write_lock.read_unlock();
        throw ServiceProviderException("Service with name '" + name + "' doesn't exist");
    }
    void* service = _services[name];
    _read_write_lock.read_unlock();
    return service;
}
