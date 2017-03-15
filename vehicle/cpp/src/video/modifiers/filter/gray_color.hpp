#ifndef gray_color_hpp
#define gray_color_hpp

#include "../abstract_modifier.hpp"
#include <opencv2/imgproc.hpp>

namespace VehicleModule{
    namespace Video {
        namespace Modifiers{
            namespace Filter{
                class GrayColor : public AbstractModifier{
                public:
                  void apply(cv::Mat &);
                };
            };
        };
    };
};

#endif
