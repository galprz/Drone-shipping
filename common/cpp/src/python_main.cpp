#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include "udp/udp_receiver.hpp"
#include "udp/udp_sender.hpp"

using namespace Common;
BOOST_PYTHON_MODULE(libcommon)
{
	int (Udp::UdpSender::*send_string)(const std::string&) const = &Udp::UdpSender::send;
	
	boost::python::class_<Udp::UdpSender>("UdpSender", boost::python::init<std::string, int>())
	.def("send", send_string);
	
	boost::python::class_<Udp::UdpReceiver>("UdpReceiver", boost::python::init<std::string, int>())
	.def("start_listen", &Udp::UdpReceiver::start_listen)
	.def("set_on_message_handler", &Udp::UdpReceiver::set_on_message_handler);
}
