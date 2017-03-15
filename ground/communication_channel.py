from websocket_server import WebsocketServer
from threading import Thread

class CommunicationChannel(WebsocketServer):

    def __init__(self, host="0.0.0.0", port=8080, cmd_client = None, udp_receiver = None):
        WebsocketServer.__init__(self,port=port,host=host)
        self.set_fn_new_client(self.__on_new_ux_client)
        self.set_fn_client_left(self.__on_ux_client_left)
        self.set_fn_message_received(self.__on_message_received_from_ux)
        self.__cmd_client = cmd_client
        self.__cmd_client.set_on_message_handler(self.__on_message_received_to_proxy)
        self.__cmd_client.set_on_error_handler(self.__on_connection_error)
        self.__cmd_client.set_on_close_handler(self.__on_connection_error)
        self.__udp_receiver = udp_receiver
        self.__udp_receiver.set_on_message_handler(self.__on_message_received_to_proxy)
        self.__udp_receiver_thread = Thread(target=self.__udp_receiver.start_listen)
        self.__udp_receiver_thread.setDaemon(True)

    def __on_new_ux_client(self,client,server):
        print "New client connected to communication channel." + str(client)

    def __on_ux_client_left(self,client,server):
        print "Client " + str(client) + " disconnected from communication channel."

    def __on_message_received_from_ux(self,client,server,msg):
        self.__cmd_client.send(msg)

    def __on_message_received_to_proxy(self, msg):
        self.send_message_to_all(msg)

    def __on_connection_error(self, error = None):
        self.send_message_to_all("Connection closed with error : " + str(error))
        self.close()

    def start(self):
        try:
            self.__udp_receiver_thread.start()
            self.run_forever()
        except Exception:
            self.close()

    def close(self):
        self.__cmd_client.close()
        self.__udp_receiver.close()
        self.shutdown()

    def __del__(self):
       self.close()