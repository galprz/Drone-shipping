#ifndef direction_vector_hpp
#define direction_vector_hpp

#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "../abstract_modifier.hpp"
#include "image_algorithm.hpp"
#include "rw_lock.hpp"
namespace VehicleModule{
    namespace Video {
        namespace Modifiers{
            namespace Mask{
                class DirectionVector : public AbstractModifier{
                    ::Common::RWLock _read_write_lock;
                    cv::Point _from;
                    cv::Point _to;
                public:
                    DirectionVector():
                    _read_write_lock(),_from(0,0), _to(0,0)
                    {}
                    void apply(cv::Mat &);
                    void set_direction_vector(const cv::Point& from, const cv::Point& to);
                };
            };
        };
    };
};

#endif
