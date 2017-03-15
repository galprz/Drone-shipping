//
//  image_algorithm.cpp
//  test
//
//  Created by Gal Malka on 23/12/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include "image_algorithm.hpp"
#include "armadillo"
using namespace std;
using namespace VehicleModule::Algorithm;

typedef double Radius;
typedef Point2f Center;
typedef double Rank;
typedef std::pair<Center,Radius> Circle;

static uchar get_median_pixel_value(const Mat& img) {
    int bucket_size = 1 << sizeof(uchar)*8;
    int bucket[bucket_size];
    for (int i=0; i < bucket_size; i++) {
        bucket[i] = 0;
    }
    for(int i=0; i < img.rows; i++) {
        for (int j=0; j < img.cols; j++) {
            bucket[img.at<uchar>(i, j)]++;
        }
    }

    int low_count = img.rows*img.cols / 10;
    int high_count = low_count;
    for (int i=0; i < bucket_size; i++) {
        int low_to_remove = min(low_count, bucket[i]);
        int high_to_remvoe = min(high_count, bucket[bucket_size - i - 1]);
        low_count -= low_to_remove;
        high_count -= high_to_remvoe;
        bucket[i] -= low_to_remove;
        bucket[bucket_size - i - 1] -= high_to_remvoe;

        if(low_to_remove == 0 && high_to_remvoe == 0){
            break;
        }
    }

    int count = 0, sum = 0;
    for (int i=0; i < bucket_size; i++) {
        count += bucket[i];
        sum += bucket[i] * i;
    }
    return sum / count;
}

static Rank rank_polygon_as_circle(const vector<Point>& polygon) {
    double perimeter = arcLength(polygon, true);
    double area = contourArea(polygon);
    return abs((perimeter * perimeter / area) - 4*M_PI);
}
static Rank rank_polyline_as_circle(const vector<Point>& polyline,const Circle & circle) {
    Rank rank = 0;
    Center center = circle.first;
    Radius radius = circle.second;
    int number_of_points = static_cast<int>(polyline.size());
    for (Point point : polyline){
        int diff = abs(radius-sqrt(pow(center.x-point.x,2) + pow(center.y-point.y,2)));
        rank += (1.0 / number_of_points) * diff;
    }
    return rank;

}

static double get_diameter(const vector<Circle>& circles, const vector<int>& indexes) {
    double diameter = 0;
    for (int i=0; i < indexes.size(); i++) {
        for (int j=0; j < indexes.size(); j++) {
            diameter = max(diameter, norm(circles[indexes[i]].first - circles[indexes[j]].first));
        }
    }
    return diameter;
}

static void get_closest_points_aux(const vector<Circle>& circles, int offset, int num_points, vector<int>& combination, double& min_diameter, vector<int>& out) {
    if (num_points == 0) {
        double diameter = get_diameter(circles, combination);
        if (diameter <= min_diameter) {
            min_diameter = diameter;
            out = combination;
        }
        return;
    }

    for (int i=offset; i <= circles.size() - num_points; ++i) {
        combination.push_back(i);
        get_closest_points_aux(circles, i+1, num_points-1, combination, min_diameter, out);
        combination.pop_back();
    }
}

static vector<Circle> get_closest_circles(const vector<Circle>& circles, int num_points, double min_diameter) {
    vector<int> indices,combination;
    vector<Circle> closest_circles;
    get_closest_points_aux(circles, 0, num_points, combination, min_diameter, indices);
    for(int i : indices){
        closest_circles.push_back(circles[i]);
    }
    return closest_circles;
}


static bool try_get_center_of_mass(const vector<Point>& polygon, Center& out) {
    Moments moments = cv::moments(polygon);
    if (moments.m00 == 0) {
        return false;
    }
    out = Center(moments.m10 / moments.m00, moments.m01 / moments.m00);
    return true;
}

static double get_radius(const vector<Point>& polygon, const Point& center_of_mass) {
    double sum = 0;
    for (Point p : polygon) {
        sum += norm(p - center_of_mass);
    }
    assert(polygon.size()!=0);
    return sum / polygon.size();
}


static vector<Circle> get_suspects_from_polygons(const vector<vector<Point>>& polygons){
    // Filter small polygons
    // TODO: set threshold
    vector<vector<Point>> matching_polygons = polygons;
    vector<Circle> circles;
    matching_polygons.erase(remove_if(matching_polygons.begin(), matching_polygons.end(), [](vector<Point> polygon){ return polygon.size() < 10; }), matching_polygons.end());
    if(!matching_polygons.size()){
        return circles;
    }
    // Find the top circle-like polygons
    int num_circle_polygons = min((int)MAX_NUM_CIRCLE_POLYGONS - 1, (int)matching_polygons.size() - 1);
    nth_element(matching_polygons.begin(), matching_polygons.begin() + num_circle_polygons, matching_polygons.end(),
                [](vector<Point> p1, vector<Point> p2) {
                    return rank_polygon_as_circle(p1) < rank_polygon_as_circle(p2);
                });

    double max_rank = rank_polygon_as_circle(matching_polygons[num_circle_polygons]);
    matching_polygons.erase(remove_if(matching_polygons.begin(), matching_polygons.end(), [&max_rank](vector<Point> polygon) {
        return rank_polygon_as_circle(polygon) > max_rank;
    }), matching_polygons.end());

    for(const vector<Point> & polygon : matching_polygons){
        Center center;
        if (try_get_center_of_mass(polygon, center)) {
            Radius radius = get_radius(polygon, center);
            if (radius >= THRESHOLD_RADIUS_MIN && radius<= THRESHOLD_RADIUS_MAX){
                Circle circle(center,radius);
                circles.push_back(circle);
            }
        }
    }

    return circles;
}

static vector<Circle> get_suspects_from_polylines(const vector<vector<Point>>& polylines){
    vector<vector<Point>> matching_polylines = polylines;
    vector<pair<Circle, Rank>> suspects;
    vector<Circle> circles;
    matching_polylines.erase(remove_if(matching_polylines.begin(), matching_polylines.end(), [](vector<Point> polyline){ return polyline.size() < 10; }), matching_polylines.end());
    if(!matching_polylines.size()){
        return circles;
    }
    for(const vector<Point> & polyline : matching_polylines) {
        vector<Point> unique_polyline;
        for (const Point & point : polyline){
            if (std::find(unique_polyline.begin(), unique_polyline.end(), point) == unique_polyline.end() ){
                unique_polyline.push_back(point);
            }
        }

        int n_rows = static_cast<int>(unique_polyline.size());
        arma::mat A(n_rows,1);
        arma::mat B(n_rows,3);
        arma::mat C(3,1);
        int count_row = 0;
        for (const Point & point : unique_polyline){
            A(count_row,0) = point.y * point.y + point.x * point.x;
            B(count_row,0) = 2 * point.x;
            B(count_row,1) = 2 * point.y ;
            B(count_row,2) = 1;
            count_row++;
        }
        arma::mat B_transpose = B.t();
        arma::mat B_tag = B_transpose * B;
        if(arma::det(B_tag) != 0)
        {
            C = arma::inv(B_tag)* (B_transpose*A);
            Center center (C(0,0),C(1,0));
            Radius radius =  sqrt(C(2,0)+ C(1,0)* C(1,0)+ C(0,0) * C(0,0));
            if(radius >= THRESHOLD_RADIUS_MIN && radius <= THRESHOLD_RADIUS_MAX){
                Circle circle(center,radius);
                std::pair<Circle,Rank> suspect(circle,rank_polyline_as_circle(unique_polyline,circle));
                suspects.push_back(suspect);
            }
        }
    }
    if(suspects.size() == 0){
        return circles;
    }
    int num_circle_polygons = min((int)MAX_NUM_CIRCLE_POLYGONS - 1, static_cast<int>(suspects.size()) - 1);
    nth_element(suspects.begin(), suspects.begin() + num_circle_polygons, suspects.end(),
                [](pair<Circle, Rank> suspect_circle1, pair<Circle, Rank> suspect_circle2) {
                    return suspect_circle1.second < suspect_circle2.second;
                });
    double max_rank = suspects[num_circle_polygons].second;
    for(const pair<Circle, Rank> & suspect : suspects){
        if (suspect.second <= max_rank){
            circles.push_back(suspect.first);
        }
    }
    return circles;
}


static bool target_center_from_circles(const vector<Circle> &circles , Point& target_center)
{
    if (circles.size() < MIN_CENTERS) {
        return false;
    }

    vector<Circle> closest_circles= get_closest_circles(circles, MIN_CENTERS, 30);
    if(!closest_circles.size()){
        return false;
    }

    std::sort(closest_circles.begin(), closest_circles.end(), [](Circle c1, Circle c2) {
        return c1.second < c2.second;
    });

    for (int i=0; i < closest_circles.size() - 1; i++) {
        if (norm(closest_circles[i].first - closest_circles[i+1].first) + closest_circles[i].second >= closest_circles[i+1].second) {
            return false;
        }
    }

    target_center = static_cast<Point>(closest_circles[0].first);
    return true;

}

bool VehicleModule::Algorithm::find_bullseye(const Mat& cv_img, Point& out) {
    // Validate that img is uchar, single channel
    if (cv_img.type() != CV_8UC1){
       // throw ImageAlgorithmException("Image is not of type uchar 1 channel");
    }

    Img<uchar> img(cv_img.rows, cv_img.cols, cv_img.data);
    ImgVectorizer0x vectorizer((uchar)0);//(get_median_pixel_value(cv_img));
    vector<vector<Point> > polygons;
    vectorizer.img2curves(img, &polygons);
    // Get suspects circles
    vector<Circle> circles = get_suspects_from_polygons(polygons);

    return target_center_from_circles(circles,out);

}

bool VehicleModule::Algorithm::find_bullseye_direction(const Mat& cv_img, Point& out){
    if (cv_img.type() != CV_8UC1){
   //     throw ImageAlgorithmException("Image is not of type uchar 1 channel");
    }
    Img<uchar> img(cv_img.rows, cv_img.cols, cv_img.data);
    ImgVectorizer0x vectorizer((uchar)0);//(get_median_pixel_value(cv_img));
    vector<vector<Point> > polygons;
    vector<vector<Point> > polylines;
    vectorizer.img2curves(img, &polylines, &polygons);
    vector<Circle> circles = get_suspects_from_polygons(polygons);
    vector<Circle> polylines_best_circles = get_suspects_from_polylines(polylines);
    for(const Circle & circle : polylines_best_circles){
        circles.push_back(circle);
    }
    
    return target_center_from_circles(circles,out);
}
