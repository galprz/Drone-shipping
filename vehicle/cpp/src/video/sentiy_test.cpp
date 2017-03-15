#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "video_provider.hpp"
#include "video_streamer.hpp"
#include <iostream>
#include <thread>

using namespace VehicleModule;
using namespace cv;

void open_stream(){
    VideoProvider & streamer = VideoProvider::get_instance();
    std::cout << "Starting camera" << '\n';
    streamer.start_listen_to_camera();
}
void send1(){
    VideoStreamer streamer("10.0.0.28",10000);
    streamer.start_sending_video();
}

void send2(){
    VideoStreamer streamer("10.0.0.28",10001);
    streamer.start_sending_video();
}
int main(){
    std::thread start_camera(open_stream);
    std::thread start_sending_1(send1);
    std::thread start_sending_2(send2);
    start_sending_2.join();
    return 0;
}
