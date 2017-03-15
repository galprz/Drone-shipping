#include "video.hpp"

bool VehicleModule::Video::send_image(const std::vector<uchar> & compressed_image, const Common::Udp::UdpSender& sender,int max_packet_size) {
    int number_of_packs_for_image = 1 + (compressed_image.size() - 1) / max_packet_size;
    int ibuf[1];
    ibuf[0] = number_of_packs_for_image;
    sender.send(ibuf, sizeof(int));
    int sent_pack_size = 0;
    for (int i = 0; i < number_of_packs_for_image; i++)
    {
        sent_pack_size += sender.send( & compressed_image[i * max_packet_size], max_packet_size);
    }
    return number_of_packs_for_image  * max_packet_size == sent_pack_size;
}
