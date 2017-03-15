#include "udp_receiver.hpp"
#define MAX_BUFFER_SIZE 8192
using namespace Common;
Udp::UdpReceiver::UdpReceiver(const std::string& addr, int port)
: port(port), addr(addr) {
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &addrinfo));
    if (r != 0 || addrinfo == NULL) {
        throw UdpReceiverRuntimeError(("invalid address or port for UDP socket: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    socket_fd = socket(addrinfo->ai_family, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        freeaddrinfo(addrinfo);
        throw UdpReceiverRuntimeError(("could not create UDP socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    r = bind(socket_fd, addrinfo->ai_addr, addrinfo->ai_addrlen);
    if (r != 0) {
        freeaddrinfo(addrinfo);
        close(socket_fd);
        throw UdpReceiverRuntimeError(("could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

Udp::UdpReceiver::~UdpReceiver() {
    freeaddrinfo(addrinfo);
    close(socket_fd);
}

int Udp::UdpReceiver::get_port() const {
    return port;
}

std::string Udp::UdpReceiver::get_addr() const {
    return addr;
}

void Udp::UdpReceiver::set_on_message_handler(PyObject* callback) {
    on_message_callback = callback;
}

void Udp::UdpReceiver::start_listen() {
    GilLock lk;
    lk.unlock();
    char buffer[MAX_BUFFER_SIZE];
    int number_of_bytes = 0;
    while (1) {
        number_of_bytes = ::recv(socket_fd, buffer, MAX_BUFFER_SIZE, 0);
        buffer[number_of_bytes] = '\0';
        lk.lock();
        boost::python::call<void>(on_message_callback, std::string(buffer));
        lk.unlock();
    }
}
