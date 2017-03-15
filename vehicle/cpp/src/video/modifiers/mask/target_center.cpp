#include "target_center.hpp"
using namespace VehicleModule::Video::Modifiers::Mask;

void TargetCenter::apply(cv::Mat& image)
{
    int radius = 5;
    int thickness = -1; //fill all the circle with color
    if (_update_exists.load()) {
        _read_write_lock.read_lock();
            cv::Point target_center = _target_center;
        _read_write_lock.read_unlock();
        if(target_center.x >= 0 && target_center.x <= image.rows && target_center.y>= 0 && target_center.y <= image.cols){
            cv::circle(image, target_center, radius, cv::Scalar(255), thickness);
            _update_exists.store(false);
        }
    }
}

void TargetCenter::set_target_center(const cv::Point & target_center)
{
    _read_write_lock.write_lock();
        _target_center = target_center;
        _update_exists.store(true);
    _read_write_lock.write_unlock();
}
