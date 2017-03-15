#include "rotate.hpp"

using namespace VehicleModule::Video::Modifiers::Transformation;
using namespace cv;

void Rotate::apply(cv::Mat & image)
{
    Mat output_image;
    Point2f pt(image.cols/2., image.rows/2.);
    Mat r = getRotationMatrix2D(pt, _angle, 1.0);
    warpAffine(image, output_image, r, Size(image.cols, image.rows));
    image = output_image;
}
