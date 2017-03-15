#include "service_provider.hpp"
#include <string>

namespace VehicleModule {
    namespace Common {
        class CppService{
        public:
            void publish_as_cpp_service(const std::string& name){
                ServiceProvider::get_instance().publish(name,this);
            }
        };
    }
};
