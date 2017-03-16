#ifndef image_streamer_hpp
#define image_streamer_hpp
#include <string>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "opencv2/opencv.hpp"
#include "video.hpp"
#include "udp/udp_sender.hpp"
#include "logger.hpp"
#include "clock.hpp"
#include "service_provider.hpp"
#include "image_streamer_config.hpp"
#include "cpp_service.hpp"
#define IMAGE_STREAMER_TAG "ImageStreamer"
/**
 * DEPRECATED
 * send images to video server
 * we dosen't use that module any more but we left it any way in case someone would need it
 */
namespace VehicleModule {
    namespace Video{
        class ImageStreamer : public Common::CppService{
        private:
            ::Common::Udp::UdpSender             _sender;
            std::queue<cv::Mat>                _queue;
            std::mutex                         _queue_mutex;
            std::condition_variable            _reader_cv;
            std::mutex                         _thread_lock;
            std::atomic<bool>                  _should_abort;
            std::atomic<bool>                  _running;
            std::atomic<int>                   _waiting_writers;
            void _stop_sending_images();
        public:
            ImageStreamer(const std::string& ip, int port) :
            _sender(ip,port),_queue(),
            _queue_mutex(),_reader_cv(),
            _should_abort(false),_running(false),
            _waiting_writers(0)
            {}
            void start_sending_images();
            void stop_sending_images();
            void send(const cv::Mat & image);
            ImageStreamer(ImageStreamer const&) = delete;
            void operator=(ImageStreamer const&) = delete;
            ~ImageStreamer();
        };
    };
};
#endif
