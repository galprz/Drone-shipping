#include "image_streamer.hpp"

using namespace VehicleModule::Video;

void ImageStreamer::send(const cv::Mat & image)
{
    if(!_running.load())
    {
        Common::Logger::warn("Trying to send image before starting the streamer",IMAGE_STREAMER_TAG);
        //return;
    }
    cv::Mat image_copy = image.clone();
    ///Notify sender that there are waiting writers
    _waiting_writers++;
    //Go to sleep on the lock until the sender will notice that this writer wants to schedule a send
    std::unique_lock<std::mutex> lk(_queue_mutex);
    bool was_empty = _queue.empty();
    _queue.push(image_copy);
    _waiting_writers--;
    lk.unlock();
    Common::Logger::debug("Schedule request to send image",IMAGE_STREAMER_TAG);
    if(was_empty || _waiting_writers.load() == 0)
    {
        _reader_cv.notify_one();
    }
}

void ImageStreamer::start_sending_images(){
    std::unique_lock<std::mutex> tlk(_thread_lock,std::defer_lock);
    ::Common::GilLock glk;
    glk.unlock();
    if(!tlk.try_lock()){
        Common::Logger::warn("Image streamer already started",IMAGE_STREAMER_TAG);
        glk.lock();
        return;
    }
    _running.store(true);
    Common::Logger::debug("Starting to stream images",IMAGE_STREAMER_TAG);
    ::Common::Clock clock;
    std::vector<uchar> encoded;
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(ENCODE_QUALITY);
    std::unique_lock<std::mutex> qlk(_queue_mutex,std::defer_lock);
    while(!_should_abort.load()){
        qlk.lock();
        while((_queue.empty() ||  _waiting_writers.load() != 0) && !_should_abort.load() )
        {
            Common::Logger::debug("Going to sleep the queue is empty or someone wanting to queue new image", IMAGE_STREAMER_TAG);
            _reader_cv.wait(qlk);
        }
        if(_should_abort.load())
        {
            qlk.unlock();
            continue;
        }
        //The queue is not empty and we dont have writes on the system so lets fetch image from the queue
        cv::Mat image = _queue.front();
        _queue.pop();
        //Now we no longer need to hold the lock so unlock it
        qlk.unlock();
        //compress the image
        cv::Mat send;
        resize(image, send, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);
        imencode(".jpg", send, encoded, compression_params);
        //Try to send image NUM_OF_RETRIES times
        int number_of_retries = 0;
        while(!Video::send_image(encoded,_sender,PACK_SIZE) && number_of_retries++ < NUM_OF_RETRIES)
        {
            Common::Logger::warn("Some packages lost in the way when sending the image",IMAGE_STREAMER_TAG);
        }
        if(number_of_retries == NUM_OF_RETRIES)
        {
            Common::Logger::warn("Giving up on trying to send the image",IMAGE_STREAMER_TAG);
        }else
        {
            Common::Logger::debug("Image sent", IMAGE_STREAMER_TAG);
        }
        clock.sleep(DELAY_TIME);
    }
    Common::Logger::info("Stop streaming images",IMAGE_STREAMER_TAG);
    _running.store(false);
    _should_abort.store(false);
    glk.lock();
    tlk.unlock();
}

void ImageStreamer::_stop_sending_images()
{
    _should_abort.store(true);
    _reader_cv.notify_one();
}

void ImageStreamer::stop_sending_images()
{
    ::Common::GilLock glk;
    glk.unlock();
    Common::Logger::info("Schedule request to stop sending images",IMAGE_STREAMER_TAG);
    _stop_sending_images();
    glk.lock();
}

ImageStreamer::~ImageStreamer()
{
    _stop_sending_images();
}
