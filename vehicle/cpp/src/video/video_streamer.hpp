#ifndef video_streamer_hpp
#define video_streamer_hpp

#include <iostream>
#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include "../common/logger.hpp"
#include "gil_lock.hpp"
#include "video.hpp"
#include "udp/udp_sender.hpp"
#include "video_provider.hpp"
#include "modifiers/transformation/resize.hpp"
#include "video_streamer_config.hpp"
#include <boost/thread/thread.hpp>

#define VIDEO_STREAMER_TAG "VideoStreamer"
/**
 * Sends the stream of the video over udp to the server
 */
namespace VehicleModule {
    namespace Video{
        class VideoStreamer {
        private:
            ::Common::Udp::UdpSender           _sender;
            std::atomic<int>                 _running;
            std::mutex                       _owner_lock;
            void _stop_sending_video();
        public:
            VideoStreamer(const std::string& addr , int port):
            _sender(addr, port),_running(false)
            {}
            /**
             * start to send the video to udp:addr:port infinite loop till stop_sending_video get called
             */
            void start_sending_video();
            /**
             * stoping the video streaming
             */
            void stop_sending_video();
            VideoStreamer(VideoStreamer const&) = delete;
            void operator=(VideoStreamer const&) = delete;
            ~VideoStreamer();
        };
    }
};

#endif /* video_streamer_hpp */
