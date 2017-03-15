#include "gray_color.hpp"

using namespace VehicleModule::Video::Modifiers::Filter;

void GrayColor::apply(cv::Mat& image)
{
    cv::Mat grey_image;
    cv::cvtColor(image, grey_image, CV_BGR2GRAY);
    image = grey_image;
}
