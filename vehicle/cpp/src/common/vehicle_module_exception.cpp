//
//  vehicle_module_exception.cpp
//  vehicle_cpp_module
//
//  Created by Gal Malka on 02/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "vehicle_module_exception.hpp"

using namespace VehicleModule::Common;
using namespace std;

const char* VehicleModuleException::what() const throw() {
    return (_name() + ": " + message).c_str();
}

string VehicleModuleException::_name() const {
    return typeid(*this).name();
}
