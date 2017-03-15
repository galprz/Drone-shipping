import socket

#TODO : change set handler to on message handler
class UdpReceiver:
    def __init__(self, port=1234, host="0.0.0.0", buffer_size=8192):
        self.__handlers = []
        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.__socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.__socket.bind((host, port))
        self.__buffer_size = buffer_size

    def set_on_message_handler(self, callback):
        self.__handlers.append(callback)

    def remove_on_message_handler(self,callback):
        if callback in self.__handlers:
            del self.__handlers[callback]

    def start_listen(self):
        while True:
            msg = self.__socket.recv(self.__buffer_size)
            for handler in self.__handlers:
                handler(msg)

    def close(self):
        self.__socket.close()

    def __del__(self):
        self.close()