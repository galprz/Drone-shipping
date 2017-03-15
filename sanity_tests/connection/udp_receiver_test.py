from common.udp_receiver import UdpReceiver


def data_incoming(data):
    print data

if __name__ == "__main__":
    receiver = UdpReceiver()
    receiver.set_on_message_handler(data_incoming)
    receiver.start_listen()
