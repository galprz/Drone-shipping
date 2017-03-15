from common.cmd_client import CMDClient
from common.types import CMDTypes
from threading import Thread

def connected():
    print "Connected"


def print_message(message):
    print message

def on_close():
    print "byes"
if __name__=="__main__":
    client = CMDClient()
    client.set_on_open_handler(connected)
    client.set_on_message_handler(print_message)
    client.set_on_close_handler(on_close)
    client.connect(True)
    client.send({"type" : CMDTypes.PING, "body" : None})
    client.send({"type":CMDTypes.GO_TO_GPS, "body" : {"lon":36.213, "lat":74.542}})
    client.send({"type": CMDTypes.ABORT_MISSION, "body": None})

    while True:
        pass
