#ifndef collection_hpp
#define collection_hpp

#include <list>
#include <memory>
#include "abstract_modifier.hpp"

namespace VehicleModule{
    namespace Video{
        namespace Modifiers{
            class Collection{
                std::list<std::shared_ptr<AbstractModifier>> _modifiers;
            public:
                Collection & add_modifier(std::shared_ptr<AbstractModifier> modifier)
                {
                    _modifiers.push_back(modifier);
                    return *this;
                };
                void apply(cv::Mat & image) const;
            };
        };
    };
};

#endif
