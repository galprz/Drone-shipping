#ifndef target_center_hpp
#define target_center_hpp

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "../abstract_modifier.hpp"
#include "image_algorithm.hpp"
#include "rw_lock.hpp"
#include <atomic>
namespace VehicleModule{
    namespace Video {
        namespace Modifiers{
            namespace Mask{
                class TargetCenter : public AbstractModifier{
                    ::Common::RWLock _read_write_lock;
                    cv::Point _target_center;
                    std::atomic<bool> _update_exists;
                public:
                    void apply(cv::Mat &);
                    void set_target_center(const cv::Point & target_center);
                };
            };
        };
    };
};

#endif
