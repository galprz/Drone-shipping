#ifndef udp_sender_hpp
#define udp_sender_hpp

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "../gil_lock.hpp"

namespace Common {
namespace Udp {
class UdpSenderRuntimeError : public std::runtime_error {
public:
    UdpSenderRuntimeError(const char* w)
    : std::runtime_error(w) {}
};
class UdpSender {

public:
    UdpSender(const std::string& addr, int port);
    ~UdpSender();
    int 				get_port() const;
    std::string 		get_addr() const;
    int 				send(const std::string& msg) const;
    int 				send(const void*, int) const;

private:
    int 				socket_fd;
    int 				port;
    std::string 		addr;
    struct addrinfo* 	addrinfo;
};
};
};

#endif /* udp_sender_hpp */
