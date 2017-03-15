#ifndef udp_receiver_hpp
#define udp_receiver_hpp

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <boost/python/call.hpp>
#include "../gil_lock.hpp"

namespace Common {
namespace Udp {
class UdpReceiverRuntimeError : public std::runtime_error {
public:
    UdpReceiverRuntimeError(const char* w)
    : std::runtime_error(w) {}
};
class UdpReceiver {
public:
    UdpReceiver(const std::string& addr, int port);
    ~UdpReceiver();
    int 				get_port() const;
    std::string 		get_addr() const;
    void 				set_on_message_handler(PyObject* callback);
    void 				start_listen();

private:
    int 				socket_fd;
    int 				port;
    std::string 		addr;
    struct addrinfo* 	addrinfo;
    PyObject* 			on_message_callback;
};
};
}


#endif /* udp_receiver_hpp */
