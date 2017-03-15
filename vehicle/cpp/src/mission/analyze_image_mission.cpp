
#include "analyze_image_mission.hpp"

using namespace VehicleModule::Mission;
using namespace VehicleModule::Video;
using namespace VehicleModule::Algorithm;
using namespace boost;

AnalyzeImageMission::AnalyzeImageMission(){
    std::cout << "mark2";
    _add_state(State(0, "AnalyzingImage", static_cast<StateMachine::st_func>(&AnalyzeImageMission::_analyzing_image),true));
}

bool AnalyzeImageMission::_analyzing_image() {
    std::cout << "mark";
    VideoProvider& video_provider = VideoProvider::get_instance();
    Modifiers::Collection collection;
    std::shared_ptr<Modifiers::AbstractModifier> target_center_mask(new Modifiers::Mask::TargetCenter());
    collection.add_modifier(target_center_mask);
    video_provider.set_channel(VideoProvider::Channel::DEBUG, collection);
    cv::Mat frame;
    cv::Point point;
    long long image_ts;
    while(1){
        auto start = std::chrono::system_clock::now();
        if (video_provider.get_frame(frame, VideoProvider::Channel::DEFAULT, image_ts)) {
            if (find_bullseye(frame, point)) {
                ;
            }
        }
        if (!video_provider.new_frame_exist(image_ts)) {
            video_provider.wait_to_next_frame();
        }
        auto end = std::chrono::system_clock::now();
        auto els = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << 1000/els << std::endl;
    };
    return false;
}
