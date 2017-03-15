//
//  serviceprovider.hpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 28/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef service_provider_hpp
#define service_provider_hpp

#include <stdio.h>
#include <unordered_map>
#include <string>
#include <boost/python/object.hpp>
#include <exception>
#include "rw_lock.hpp"
#include "vehicle_module_exception.hpp"
#include "logger.hpp"

using namespace std;

namespace VehicleModule {
    namespace Common {
        class ServiceProvider {
        private:
            unordered_map<string, void*>        _services;
            ::Common::RWLock                      _read_write_lock;
            ServiceProvider() {}
            
        public:
            static ServiceProvider& get_instance() {
                static ServiceProvider instance;
                return instance;
            }
            
            void publish(const string& name, PyObject* service);
            void publish(const string& name, void* service);
            void * get_service(const string& name);
            
            struct ServiceProviderException : public VehicleModuleException {
                ServiceProviderException(const string& message) : VehicleModuleException(message) {}
            };
            
            ServiceProvider(ServiceProvider const&) = delete;
            void operator=(ServiceProvider const&)  = delete;
        };
    }
}

#endif /* service_provider_hpp */
