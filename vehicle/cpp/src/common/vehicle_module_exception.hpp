//
//  vehicle_module_exception.hpp
//  vehicle_cpp_module
//
//  Created by Gal Malka on 02/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef vehicle_module_exception_hpp
#define vehicle_module_exception_hpp

#include <stdio.h>
#include <string>
#include <typeinfo>

using namespace std;

namespace VehicleModule {
    namespace Common {
        class VehicleModuleException : public std::exception {
        private:
            string _name() const;
        public:
            string message;
            
            explicit VehicleModuleException(const string& message) : message(message) {}
            explicit VehicleModuleException() {}
            virtual const char* what() const throw();
        };
    }
}

#endif /* vehicle_module_exception_hpp */
