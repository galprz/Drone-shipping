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

/**
 * Video provider takes the stream of the camera and provide it to any consumer
 * that way there is only one component in the system that reads the stream of the camera
 *
 * Channels:
 * we divid the stream of the video to channels the consumers of the video can read the
 * stream from the deafult channel that will provide them the video without any filters or masks
 * in the other hands consumers can read the stream from diffrent channel for example the video_streamer
 * consum the stream from DEBUG channel so before the video provider provides the video it apply the gray mask that
 * transform the image from RBG image to gray scale image . in addition you can see in the find and land mission when the drone
 * is in state 'scan' we load the DEBUG channel with 'target center mask' that way when the algorithem finds the target in the frame
 * it will paint the center of the target with white circle then the video stream will consume the image and will
 * send the frame with the mask already applied.
 * you can add your own channels if needed
 */
namespace VehicleModule {
    namespace Video{
        class VideoProvider {
        public:
            /**
             * DEFAULT channel for consumers that want the video stream without any changes
             * DEBUG adding masks and filter to the image for debugging
             */
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
            /**
             * get the width of the video
             * @return width of the video in pixels
             */
            int  get_width() const;
            /**
             * get the height of the video
             * @return height of the video in pixels
             */
            int  get_height() const;
            /**
             * start listening to the camera the thread get into infinint loop and fetch frames
             * from camera.
             */
            void start_listen_to_camera();
            /**
             * stop to listening to the camera
             */
            void stop_listen_to_camera();
            /**
             * get the current frame from specific channel
             * @param frame   the function set this variable to the last frame that we got from camera
             * @param channel if we featch from diffrent channel other then default we apply the collection that channel has from _channels
             * @return true if we got frame from camera else false
             */
            bool get_frame(cv::Mat& frame, Channel channel = Channel::DEFAULT);
            /**
            * get the current frame from specific channel and mark the timestamp
            * @param frame           the function set this variable to the last frame that we got from camera
            * @param channel         if we featch from diffrent channel other then default we apply the collection that channel has from _channels
            * @param image_timestamp mark the timestamp when this image took from camera that way we can use the 'new_frame_exist' function to
            *                        check if there is a new frame or to wait till we get notification about new frame with 'wait_to_next_frame'
            * @return true if we got frame from camera else false
            */
            bool get_frame(cv::Mat& frame, Channel channel , long long & image_timestamp );
            /**
            * add collection to channel collection contains filters masks and transformations see the modifiers folder for more info
            * @param Channel 2 channels exists DEFAULT and DEBUG DEFAULT provides the original frame and DEBUG apply the collection of modifiers if we loaded the with this function
            *                Note : you cannot load collection to the DEFAULT channel if you need diffrent channel other then DEBUG just create one
            * @param [name]  [description]
            */
            void set_channel(Channel , const Modifiers::Collection & );
            /**
             * clear channel from collection
             * @param channel the channel you want to clear
             */
            void clear_channel(Channel channel);
            /**
             * tells you if new frame with diffrent timestamp exists (already featched from camera)
             * @param  old_image_timestamp timestamp you get when you call get_frame
             * @return true if new frame exists else false
             */
            bool new_frame_exist(long long old_image_timestamp);
            /**
             * provides a way to suspend the current thread till new frame exists
             */
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
