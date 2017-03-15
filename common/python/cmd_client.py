import json
import websocket
from websocket import WebSocketApp
from enum import Enum
from types import CMDTypes
from threading import Thread

#TODO : add on message handler function

# CMDClient Exceptions

class RemoveHandlerError(Exception):
    pass


class SendCommandError(Exception):
    pass


class CommunicationError(Exception):
    pass


class CMDClient(WebSocketApp):

    class Events(Enum):
        ON_MESSAGE = 1,
        ON_ERROR = 2,
        ON_CLOSE = 3,
        ON_OPEN = 4

    class State(Enum):
        DISCONNECTED = 1,
        CONNECTED = 2,
        TRYING_TO_CONNECT = 3

    def __init__(self,ip="127.0.0.1",port=3128):

        websocket.enableTrace(True)
        WebSocketApp.__init__(self,"ws://" + ip + ":" + str(port),
                                         on_message=self.__on_message,
                                         on_error = self.__on_error,
                                         on_close = self.__on_close)
        self.on_open = self.__on_open
        self.__handlers = {}
        self.__client_state = CMDClient.State.DISCONNECTED
        self.__valid_cmd_type = {}
        for valid_cmd in list(CMDTypes):
            self.__valid_cmd_type[str(valid_cmd)] = valid_cmd

    def __on_message(self, ws, message):
        if CMDClient.Events.ON_MESSAGE in self.__handlers:
            self.__handlers[CMDClient.Events.ON_MESSAGE](message)

    def __on_error(self, ws, error):
        self.close()
        if CMDClient.Events.ON_ERROR in self.__handlers:
            self.__handlers[CMDClient.Events.ON_ERROR](error)

    def __on_close(self, ws):
        self.close()
        if CMDClient.Events.ON_CLOSE in self.__handlers:
            self.__handlers[CMDClient.Events.ON_CLOSE]()

    def __on_open(self, ws):
        self.__client_state = CMDClient.State.CONNECTED
        if CMDClient.Events.ON_OPEN in self.__handlers:
            self.__handlers[CMDClient.Events.ON_OPEN]()

    def __validate_cmd(self,cmd):
        types = self.__valid_cmd_type
        if "type" not in cmd or "body" not in cmd:
            return False
        if isinstance(cmd["type"],unicode):
            cmd["type"] = str(cmd["type"])
        if isinstance(cmd["type"],str) and cmd["type"] in types:
            cmd["type"] = self.__valid_cmd_type[cmd["type"]]
        if type(cmd["type"]) is not CMDTypes:
            return False
        return True

    def send(self,cmd):
        if isinstance(cmd,str) :
            cmd = json.loads(cmd)
        if self.__client_state is not CMDClient.State.CONNECTED:
            raise SendCommandError("Client is not connected to command server")
        if not self.__validate_cmd(cmd):
            raise SendCommandError("Illegal command")
        cmd['type'] = str(cmd['type'])
        WebSocketApp.send(self,json.dumps(cmd))

    def connect(self,block = False):
        self.__client_state = CMDClient.State.TRYING_TO_CONNECT
        self.__connection_thread = Thread(target=self.run_forever)
        self.__connection_thread.setDaemon(True)
        self.__connection_thread.start()
        if block:
            while self.__client_state == CMDClient.State.TRYING_TO_CONNECT:
                pass
            if self.__client_state != CMDClient.State.CONNECTED:
                raise CommunicationError("Cannot connect to cmd server")


    def state(self):
        return self.__client_state

    def set_on_open_handler(self,handler):
        self.__handlers[CMDClient.Events.ON_OPEN] = handler

    def set_on_close_handler(self,handler):
        self.__handlers[CMDClient.Events.ON_CLOSE] = handler

    def set_on_error_handler(self, handler):
        self.__handlers[CMDClient.Events.ON_ERROR] = handler

    def set_on_message_handler(self,handler):
        self.__handlers[CMDClient.Events.ON_MESSAGE] = handler

    def remove_handler(self,handler_type):
        if type(handler_type) is not CMDClient.Events:
            raise RemoveHandlerError("Illegal handler type , handler_type need to be of type CMDClient.Handlers")
        if handler_type in self.__handlers:
            del self.__handlers[handler_type]

    def close(self):
        WebSocketApp.close(self)
        self.__client_state = CMDClient.State.DISCONNECTED

    def __del__(self):
        self.close()
