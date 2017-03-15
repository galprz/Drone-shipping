#include "resize.hpp"

using namespace VehicleModule::Video::Modifiers::Transformation;

void Resize::apply(cv::Mat & image)
{
    cv::Mat output_image;
    if(image.rows != _height || image.cols != _width){
        resize(image, output_image, cv::Size(_width, _height), 0, 0, cv::INTER_LINEAR);
        image = output_image;
    }
}
