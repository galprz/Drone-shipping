#include "collection.hpp"

using namespace VehicleModule::Video::Modifiers;

void Collection::apply(cv::Mat & image) const
{
    for(const std::shared_ptr<AbstractModifier> modifier : _modifiers)
    {
        modifier->apply(image);
    }
}
