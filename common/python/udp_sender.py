import socket


class UdpSender:
    def __init__(self):
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    def send(self, msg, port=1234, host="127.0.0.1"):
        return self.__socket.sendto(msg,(host,port))
