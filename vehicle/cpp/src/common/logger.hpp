//
//  logger.hpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 25/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef logger_hpp
#define logger_hpp

#include <boost/python/call_method.hpp>
#include <iostream>
#include "thread_locks.hpp"
#include "service_provider.hpp"
#include <thread>

using namespace boost;
using namespace std;

namespace VehicleModule {
    namespace Common {
        // Wrapper class for python logging.Logger
        class Logger{
        private:
            static void _log(const string& msg, char const* level, const string& tag);
        public:
            Logger() = delete;
            
            static void info(const string& msg, const string& tag);
            static void warn(const string& msg, const string& tag);
            static void critical(const string& msg, const string& tag);
            static void debug(const string& msg, const string& tag);
            static void state(int id, const string& description, const string& tag);
        };
    }
}

#endif /* logger_hpp */
