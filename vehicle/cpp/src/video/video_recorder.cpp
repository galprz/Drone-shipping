#include "video_recorder.hpp"

using namespace VehicleModule::Video;
#define WAIT_TO_VIDEOPROVIDER  500
#define NUMBER_OF_RETRIES 5

void VideoRecorder::start_recording_video()
{
    ::Common::GilLock glk;
    glk.unlock();
    std::unique_lock<std::mutex> lk(_owner_lock,std::defer_lock);
    if(!lk.try_lock())
    {
        Common::Logger::warn("Video recorder already started",VIDEO_RECORDER_TAG);
        glk.lock();
        return;
    }
    _running.store(true);
    VideoProvider& provider = VideoProvider::get_instance();
    int retries = 0;
    while((!provider.get_width() || !provider.get_height()) && retries++ < NUMBER_OF_RETRIES){
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TO_VIDEOPROVIDER));
    }
    int frame_width  = provider.get_width();
    int frame_height = provider.get_height();
    if(!frame_width || !frame_height){
        Common::Logger::critical("Cannot get the width and height of the frame from video provider",VIDEO_RECORDER_TAG);
        throw VideoRecorderException("Cannot get the width and height of the frame from video provider");
    }
    cv::Mat frame;
    VideoWriter video_writer(_filename,CV_FOURCC('F', 'M', 'P', '4') ,15, Size(frame_width,frame_height),false);
    if(!video_writer.isOpened()){
        Common::Logger::critical("Cannot open video recorder",VIDEO_RECORDER_TAG);
        throw VideoRecorderException("Cannot open video recorder");
    }
    Common::Logger::info("Strat recording video",VIDEO_RECORDER_TAG);
    bool already_warned = false;
    while(_running.load()){
        long long frame_timestamp = 0;
        if(provider.get_frame(frame,VideoProvider::Channel::DEFAULT,frame_timestamp))
        {
            video_writer.write(frame);
            already_warned = false;
        }
        else
        {
            if(!already_warned)
            {
                Common::Logger::warn("Cannot fetch frame from camera",VIDEO_RECORDER_TAG);
                already_warned = true;
            }
        }
        if(!provider.new_frame_exist(frame_timestamp)){
            provider.wait_to_next_frame();
        }
    }
    Common::Logger::info("Stop recording video",VIDEO_RECORDER_TAG);
    video_writer.release();
}

void VideoRecorder::stop_recording_video(){
    ::Common::GilLock lk;
    lk.unlock();
    _stop_recording_video();
    lk.lock();
}

void VideoRecorder::_stop_recording_video(){
    if(_running.load())
    {
        Common::Logger::info("Schedule request to stop recording video",VIDEO_RECORDER_TAG);
        _running.store(false);
    }
}

VideoRecorder::~VideoRecorder(){
    _stop_recording_video();
}
