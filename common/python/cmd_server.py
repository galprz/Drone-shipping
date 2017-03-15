import json
from websocket_server import WebsocketServer
from enum import Enum
from types import CMDTypes


# CMDServer Exceptions
class SetMessageHandlerError(Exception):
    pass


class ParseCommandError(Exception):
    pass


class CMDServer(WebsocketServer):

    class Handlers(Enum):
        ON_NEW_CLIENT = 1,
        ON_CLIENT_LEFT = 2,
        ON_MESSAGE = 3

    def __init__(self,host="0.0.0.0",port=3128):
        WebsocketServer.__init__(self,port=port,host=host)
        self.set_fn_new_client(self.__on_new_client)
        self.set_fn_client_left(self.__on_client_left)
        self.set_fn_message_received(self.__on_message_received)
        self.__connection_handlers = {}
        self.__message_handlers = {}

    def __on_new_client(self,client,server):
        if CMDServer.Handlers.ON_NEW_CLIENT in self.__connection_handlers:
            self.__connection_handlers[CMDServer.Handlers.ON_NEW_CLIENT](client)

    def __on_client_left(self,client,server):
        if CMDServer.Handlers.ON_CLIENT_LEFT in self.__connection_handlers:
            self.__connection_handlers[CMDServer.Handlers.ON_CLIENT_LEFT](client)

    def __on_message_received(self,client,server,message):
        try:
            json_message = json.loads(message)
        except ValueError as e:
            self.send_message(client,"Message is not a valid command : " + message)
            # should not get here
            self.send_message_to_all("Error parsing json : " + message)
            raise ParseCommandError("Message is not in json format")
        if "type" not in json_message or "body" not in json_message:
            self.send_message(client, "message is not a valid command : " + message)
            # should not get here
            self.send_message_to_all("Error parsing command : " + message)
            raise ParseCommandError("Cannot find command type or command body")
        message_type = json_message["type"]
        message_body = json_message["body"]
        if message_type in self.__message_handlers:
            self.__message_handlers[message_type](message_body)
        self.send_message_to_all(message)

    def set_on_open_handler(self, callback):
        self.__connection_handlers[CMDServer.Handlers.ON_NEW_CLIENT] = callback

    def set_on_close_handler(self, callback):
        self.__connection_handlers[CMDServer.Handlers.ON_CLIENT_LEFT] = callback

    def set_on_message_handler(self, cmd_type, callback):
        if type(cmd_type) is not CMDTypes:
            raise SetMessageHandlerError("Illegal command type , cmd_type need to be of type CMDTypes")
        self.__message_handlers[str(cmd_type)] = callback

    def __del__(self):
        self.shutdown()
