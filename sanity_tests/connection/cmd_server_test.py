from common.cmd_server import CMDServer
from common.types import CMDTypes
from threading import Thread
from time import sleep

def ping_received(cmd):
    print "ping from client"


def go_to_gps(args):
    print "go to gps : lat = " + str(args["lat"]) + " lon = " + str(args["lon"])


def abort(args):
    print "mission aborted"


def new_client(client):
    print "new client " + str(client)


def client_left(client):
    print "client " + str(client) + " left"


if __name__ == "__main__":
    cmd_server = CMDServer()
    cmd_server.set_on_open_handler(new_client)
    cmd_server.set_on_message_handler(CMDTypes.PING, ping_received)
    cmd_server.set_on_message_handler(CMDTypes.GO_TO_GPS, go_to_gps )
    cmd_server.set_on_message_handler(CMDTypes.ABORT_MISSION, abort)
    cmd_server.set_on_close_handler(client_left)
    cmd_server_thread = Thread(target=cmd_server.run_forever)
    cmd_server_thread.setDaemon(True)
    cmd_server_thread.start()
    sleep(15)
    print "now"
    cmd_server.send_message_to_all("################################test server messages#####################")
    while True:
        pass