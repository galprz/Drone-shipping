#include "video_provider.hpp"

using namespace VehicleModule::Video;

void VideoProvider::start_listen_to_camera(){
    ::Common::GilLock glk;
    glk.unlock();
    //Trying to become the thread that runs the camera stream
    std::unique_lock<std::mutex> lk(_owner_lock,std::defer_lock);
    if(!lk.try_lock())
    {
        Common::Logger::warn("Video provider already started" , VIDEO_PROVIDER_TAG);
        glk.lock();
        return;
    }
    // Try to open the camera if not open already
    if(!(_camera.isOpened()) && !(_camera.open(CAMERA_INDEX)))
    {
        _running.store(false);
        Common::Logger::critical("Cannot find camera" , VIDEO_PROVIDER_TAG);
        glk.lock();
        lk.unlock();
        throw VideoProviderException("Cannot find camera");
    }
    //Set the suggested frame size
    _camera.set(CV_CAP_PROP_FRAME_WIDTH,CAMERA_WIDTH);
    _camera.set(CV_CAP_PROP_FRAME_HEIGHT,CAMERA_HEIGHT);
    _codec_type = static_cast<int>(_camera.get(CV_CAP_PROP_FOURCC));

    Common::Logger::info("Camera connected starting to read frames" , VIDEO_PROVIDER_TAG);
    //Take the next frame index to write to
    int next_image = _current_frame ^ 1;
    int corrupted_data = 0;
    bool first_run = true;
    ::Common::Clock _clock;
    while(first_run || _running.load()){
        //Take the next frame from the camera
        if(!(_camera.read(_frames[next_image]) || corrupted_data >= CORRUPTED_DATA_LIMIT))
        {
            _running.store(false);
            if(_camera.isOpened()){
                _camera.release();
            }

            if(corrupted_data >= CORRUPTED_DATA_LIMIT)
            {
                Common::Logger::critical("Got corrupted data from camera too many times" , VIDEO_PROVIDER_TAG);
                glk.lock();
                lk.unlock();
                throw VideoProviderException("Got corrupted data from camera");
            }

            Common::Logger::critical("Cannot find camera" , VIDEO_PROVIDER_TAG);
            glk.lock();
            lk.unlock();
            throw VideoProviderException("Cannot find camera" );
        }

        //Check if the data is ok
        if(_frames[next_image].size().width==0)
        {
            corrupted_data++;
            Common::Logger::warn("Got corrupted data from camera" , VIDEO_PROVIDER_TAG);
            continue;
        }

        corrupted_data = 0;
        _read_write_lock.write_lock();
            _current_frame = next_image;
            next_image = next_image ^ 1;
            struct timeval tp;
            gettimeofday(&tp, NULL);
            _current_frame_timestamp = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        _read_write_lock.write_unlock();
        if(first_run){
            //Got the first frame and now the video provider able to supply frames to readers
            _width  = _frames[_current_frame].cols;
            _height = _frames[_current_frame].rows;
            _running.store(true);
            first_run = false;
        }
        //we have new frame wake all the threads that wait for it
        std::unique_lock<std::mutex> wlk(_wait_to_next_frame_lock);
            _wait_to_next_frame_cv.notify_all();
        wlk.unlock();
        _clock.sleep(1000/FRAME_PER_SEC);
    }
    Common::Logger::info("Stop listening to camera" , VIDEO_PROVIDER_TAG);
    _current_frame_timestamp = 0;
    std::unique_lock<std::mutex> wlk(_wait_to_next_frame_lock);
        _wait_to_next_frame_cv.notify_all();
    wlk.unlock();
    glk.lock();
    lk.unlock();
}

void VideoProvider::stop_listen_to_camera(){
    ::Common::GilLock lk;
    lk.unlock();
    _stop_listen_to_camera();
    lk.lock();
}

void VideoProvider::_stop_listen_to_camera(){
    if(_running.load())
    {
        Common::Logger::info("Schedule request to stop listening to camera" , VIDEO_PROVIDER_TAG);
        _running.store(false);
    }
}
/**
return : the return value is the frame`s timestamp e.g when the frame was pulled from camera
         0 if error occur or we haven`t fetched any frame from camera
*/
bool VideoProvider::get_frame(cv::Mat& frame , Channel channel){
    if(_running.load())
    {
        _read_write_lock.read_lock();
        frame = _frames[_current_frame].clone();
        _read_write_lock.read_unlock();
        if(channel != Channel::DEFAULT)
        {
            //Apply the modifications
            Modifiers::Collection collection;
            _channels_read_write_lock.read_lock();
            bool found_collection = _channels.find(channel) != _channels.end();
            if(found_collection)
            {
                collection = _channels[channel];
            }
            _channels_read_write_lock.read_unlock();
            if(found_collection)
            {
                collection.apply(frame);
            }
        }
        return true;
    }
    return false;
}
bool VideoProvider::get_frame(cv::Mat& frame , Channel channel,long long& image_timestamp){
    bool got_frame = VideoProvider::get_frame(frame,channel);
    if(got_frame){
        _read_write_lock.read_lock();
        image_timestamp = _current_frame_timestamp;
        _read_write_lock.read_unlock();
    }
    return got_frame;
}

void VideoProvider::set_channel(Channel channel,const Modifiers::Collection & collection)
{
    if(channel != Channel::DEFAULT){
        _channels_read_write_lock.write_lock();
        _channels[channel] = collection;
        _channels_read_write_lock.write_unlock();
    }
}

void VideoProvider::clear_channel(Channel channel)
{
    _channels_read_write_lock.read_lock();
    bool found_channel = _channels.find(channel) != _channels.end();
    _channels_read_write_lock.read_unlock();

    if(found_channel){
        _channels_read_write_lock.write_lock();
        _channels.erase(channel);
        _channels_read_write_lock.write_unlock();
    }
}

bool VideoProvider::new_frame_exist(long long old_image_timestamp){
    _read_write_lock.read_lock();
    long long current_timestamp = _current_frame_timestamp;
    _read_write_lock.read_unlock();
    return current_timestamp > old_image_timestamp;
}
void VideoProvider::wait_to_next_frame(){
    if(_running.load()){
        std::unique_lock<std::mutex> lk(_wait_to_next_frame_lock);
        _wait_to_next_frame_cv.wait_for(lk, std::chrono::seconds(1));
    }
}

int VideoProvider::get_width() const{
    return _width;
}

int VideoProvider::get_height() const{
    return _height;
}

VideoProvider::~VideoProvider(){
    _stop_listen_to_camera();
    _frames[0].release();
    _frames[1].release();
    if(_camera.isOpened()){
        _camera.release();
    }

}
