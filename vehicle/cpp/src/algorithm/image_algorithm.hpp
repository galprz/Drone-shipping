//
//  image_algorithm.hpp
//  test
//
//  Created by Gal Malka on 23/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#ifndef image_algorithm_hpp
#define image_algorithm_hpp


#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include "Img.h"
#include "ImgVectorizer.h"
#include <list>
#include <algorithm>
#include <armadillo>

using namespace cv;

#define MAX_NUM_CIRCLE_POLYGONS 20
#define MAX_NUM_CIRCLE_POLYLINES 20
#define THRESHOLD_RADIUS_MIN 5
#define THRESHOLD_RADIUS_MAX 10000000
#define MIN_CENTERS 3
/**
 * we provide two built in image algorithms . you can add you own here
 *
 */
namespace VehicleModule {
    namespace Algorithm {

        /**
         * finds a bullseye target's center only if all the target in the frame
         * that means the target's center must be inside the frame unlike 'find_bullseye_direction'
         * @param  img  input image
         * @param  out  center of target in pixels if we found one
         * @return true if found target else false
         */
        bool find_bullseye(const Mat& img, Point& out);
        /**
         * finds a bullseye target's center even if we have just part of the target in the frame
         * that means the target's center could be outside the frame
         * @param  img  input image
         * @param  out  center of target in pixels if we found one
         * @return true if found target else false
         */
        bool find_bullseye_direction(const Mat& img, Point& out);
    }
}

#endif /* image_algorithm_hpp */
