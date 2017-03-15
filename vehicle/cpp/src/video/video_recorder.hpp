#ifndef video_recorder_hpp
#define video_recorder_hpp
#include <atomic>
#include <mutex>
#include <chrono>
#include <opencv2/videoio.hpp>
#include "gil_lock.hpp"
#include "video_provider.hpp"
#include "../common/logger.hpp"
#include "../common/vehicle_module_exception.hpp"

#define VIDEO_RECORDER_TAG "VideoRecoder"

namespace VehicleModule {
    namespace Video{
        class VideoRecorder {
        private:
            std::atomic<int>                 _running;
            std::mutex                       _owner_lock;
            std::string                      _filename;
            void _stop_recording_video();
        public:
            VideoRecorder(const std::string& filename):
            _running(false),
            _filename(filename)
            {}
            void start_recording_video();
            void stop_recording_video();
            VideoRecorder(VideoRecorder const&) = delete;
            void operator=(VideoRecorder const&) = delete;
            ~VideoRecorder();
            struct VideoRecorderException : public Common::VehicleModuleException{
                VideoRecorderException(const string& message): VehicleModuleException(message){}
            };
        };
    }
}

#endif
