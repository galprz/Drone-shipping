#include "direction_vector.hpp"
using namespace VehicleModule::Video::Modifiers::Mask;

void DirectionVector::apply(cv::Mat& image)
{
    cv::Point from,to;
    _read_write_lock.read_lock();
        from = _from;
        to = _to;
    _read_write_lock.read_unlock();
    int thickness = 3;
    cv::arrowedLine (image, from, to, cv::Scalar(255), thickness);
}

void DirectionVector::set_direction_vector(const cv::Point& from, const cv::Point& to)
{
    _read_write_lock.write_lock();
        _from = from;
        _to = to;
    _read_write_lock.write_unlock();
}
