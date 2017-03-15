#ifndef video_provider_hpp
#define video_provider_hpp

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <iostream>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <sys/time.h>
#include <boost/thread/thread.hpp>
#include <chrono>
#include "../common/logger.hpp"
#include "../common/vehicle_module_exception.hpp"
#include "rw_lock.hpp"
#include "gil_lock.hpp"
#include "clock.hpp"
#include "modifiers/collection.hpp"
#include "modifiers/filter/gray_color.hpp"
#include "modifiers/mask/target_center.hpp"
#include "video_provider_config.hpp"

#define VIDEO_PROVIDER_TAG "VideoProvider"



namespace VehicleModule {
    namespace Video{
        class VideoProvider {
        public:
            enum class Channel
            {
                DEFAULT,
                DEBUG
            };
        private:
            struct ChannelHash
            {
                std::size_t operator()(const Channel &  c) const
                {
                    return static_cast<std::size_t>(c);
                }
            };

            cv::VideoCapture        _camera;
            ::Common::RWLock        _read_write_lock;
            std::atomic_bool        _running;
            std::mutex              _owner_lock;
            std::mutex              _wait_to_next_frame_lock;
            std::condition_variable _wait_to_next_frame_cv;
            cv::Mat                 _frames[2];
            int                     _current_frame;
            int                     _width;
            int                     _height;
            int                     _codec_type;
            long long               _current_frame_timestamp;
            ::Common::RWLock        _channels_read_write_lock;
            std::unordered_map<Channel, Modifiers::Collection , ChannelHash> _channels;
            VideoProvider():
            _camera(),_read_write_lock(),
            _frames(),_current_frame(0),
            _running(false),_width(0),_height(0),_codec_type(0),
            _channels(),_channels_read_write_lock()
            { };
            void _stop_listen_to_camera();
        public:
            static VideoProvider& get_instance() {
                static VideoProvider instance;
                return instance;
            }
            int  get_width() const;
            int  get_height() const;
            int  get_codec_type() const;
            void start_listen_to_camera();
            void stop_listen_to_camera();
            bool get_frame(cv::Mat& , Channel channel = Channel::DEFAULT);
            bool get_frame(cv::Mat& , Channel channel , long long & image_timestamp );
            void set_channel(Channel , const Modifiers::Collection & );
            void clear_channel(Channel channel);
            bool new_frame_exist(long long old_image_timestamp);
            void wait_to_next_frame();
            VideoProvider(VideoProvider const&) = delete;
            void operator=(VideoProvider const&) = delete;
            ~VideoProvider();

            struct VideoProviderException : public Common::VehicleModuleException{
                VideoProviderException(const string& message): VehicleModuleException(message){}
            };

        };

    };
}


#endif /* vide_provider_hpp */
