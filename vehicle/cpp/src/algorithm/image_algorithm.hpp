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

namespace VehicleModule {
    namespace Algorithm {
  
        
        bool find_bullseye(const Mat& img, Point& out);
        bool find_bullseye_direction(const Mat& img, Point& out);
    }
}

#endif /* image_algorithm_hpp */
