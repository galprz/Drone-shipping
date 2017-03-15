#include "udp_sender.hpp"

using namespace Common;

Udp::UdpSender::UdpSender(const std::string& addr, int port)
: port(port),
addr(addr)
{
	char decimal_port[16];
	snprintf(decimal_port, sizeof(decimal_port), "%d", port);
	decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &addrinfo));
	if(r != 0 || addrinfo == NULL)
	{
		throw UdpSenderRuntimeError(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
	}
	socket_fd = socket(addrinfo->ai_family, SOCK_DGRAM , IPPROTO_UDP);
	if(socket_fd == -1)
	{
		freeaddrinfo(addrinfo);
		throw UdpSenderRuntimeError(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
	}
}

Udp::UdpSender::~UdpSender(){
	freeaddrinfo(addrinfo);
	close(socket_fd);
}

int Udp::UdpSender::get_port() const
{
	return port;
}

std::string Udp::UdpSender::get_addr()  const
{
	return addr;
}

int Udp::UdpSender::send(const std::string& msg) const
{
	GilLock lk;
	lk.unlock();
	int bytes = sendto(socket_fd, msg.c_str(), msg.size(), 0, addrinfo->ai_addr, addrinfo->ai_addrlen);
	lk.lock();
	return bytes;
}

int Udp::UdpSender::send(const void *buffer, int len) const
{
	return sendto(socket_fd, (char *) buffer, len, 0, addrinfo->ai_addr, addrinfo->ai_addrlen);
}
