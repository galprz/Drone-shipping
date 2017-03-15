#ifndef resize_hpp
#define resize_hpp

#include "../abstract_modifier.hpp"
#include <opencv2/imgproc.hpp>

namespace VehicleModule{
    namespace Video {
        namespace Modifiers{
          namespace Transformation{
            class Resize : public AbstractModifier{
                int _width;
                int _height;
            public:
                Resize(int width ,int height) : _width(width),_height(height){};
                void apply(cv::Mat &);
            };
          };
        };
    };
};
#endif
