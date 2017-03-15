#ifndef rotate_hpp
#define rotate_hpp

#include "../abstract_modifier.hpp"
#include <opencv2/imgproc.hpp>

namespace VehicleModule{
    namespace Video {
        namespace Modifiers{
          namespace Transformation{
            class Rotate : public AbstractModifier{
                int _angle;
            public:
                Rotate(int angle) : _angle(angle){};
                void apply(cv::Mat &);
            };
          };
        };
    };
};

#endif
