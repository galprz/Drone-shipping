#include "video_streamer.hpp"

using namespace VehicleModule::Video;

void VideoStreamer::start_sending_video(){
    ::Common::GilLock glk;
    glk.unlock();
    std::unique_lock<std::mutex> lk(_owner_lock,std::defer_lock);
    if(!lk.try_lock())
    {
        Common::Logger::warn("Video streamer already started",VIDEO_STREAMER_TAG);
        glk.lock();
        return;
    }
    std::vector< uchar > encoded;
    std::vector< int > compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(ENCODE_QUALITY);
    _running.store(true);
    VideoProvider& provider = VideoProvider::get_instance();
    cv::Mat frame,send;
    int total_pack = 0;
    int ibuf[1];
    Common::Logger::info("Start sending video",VIDEO_STREAMER_TAG);
    bool already_warned = false;
    ::Common::Clock clock;
    Modifiers::Transformation::Resize resizeTransformer(FRAME_WIDTH,FRAME_HEIGHT);
    while(_running.load()){
        long long frame_timestamp = 0;
        if(provider.get_frame(frame,VideoProvider::Channel::DEBUG,frame_timestamp))
        {
            resizeTransformer.apply(frame);
            imencode(".jpg", frame, encoded, compression_params);
            if(!Video::send_image(encoded,_sender,PACK_SIZE))
            {
                Common::Logger::warn("Some packages lost in the way when sending the frame",VIDEO_STREAMER_TAG);
            }
            already_warned = false;
        }
        else
        {
            if(!already_warned)
            {
                Common::Logger::warn("Cannot fetch frame from camera",VIDEO_STREAMER_TAG);
                already_warned = true;
            }
        }
        if(!provider.new_frame_exist(frame_timestamp)){
            provider.wait_to_next_frame();
        }
    }
    Common::Logger::info("Stop sending video",VIDEO_STREAMER_TAG);
    glk.lock();
    lk.unlock();
}

void VideoStreamer::stop_sending_video(){
    ::Common::GilLock lk;
    lk.unlock();
    _stop_sending_video();
    lk.lock();
}

void VideoStreamer::_stop_sending_video(){
    if(_running.load())
    {
        Common::Logger::info("Schedule request to stop sending video",VIDEO_STREAMER_TAG);
        _running.store(false);
    }
}

VideoStreamer::~VideoStreamer(){
    _stop_sending_video();
}
