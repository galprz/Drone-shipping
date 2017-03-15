#ifndef video_hpp
#define video_hpp

#include <vector>
#include "udp/udp_sender.hpp"
#include <opencv2/opencv.hpp>

namespace VehicleModule{
    namespace Video{
        bool send_image(const std::vector<uchar> & compressed_image, const ::Common::Udp::UdpSender& sender,int max_packet_size);
    };
};
#endif
