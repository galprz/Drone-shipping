from Queue import Queue
from common.cpp.build.libcommon import UdpSender
from threading import Thread
import datetime
import os
import json
from types import LogTypes
from types import StatusTypes

'''
  Logs to UDP, file, output
'''
class _Logger:
    def __init__(self):
        self.__log_queue = Queue()
        self.__status_queue = Queue()
        self.__udp_client = None
        self.__cmd_server = None
        if not os.path.exists("logs"):
            os.makedirs("logs")
        self.__file = open(
            "logs/" + str(datetime.datetime.now().strftime("%d-%m-%YT%H.%M.%S")), "a", 0)

        def pull_logs():
            while True:
                log = self.__log_queue.get(block=True)
                try:
                    print log["body"]
                    if self.__udp_client is not None:
                        self.__udp_client.send(json.dumps(log))

                    self.__file.write(log["body"] + "\n")
                except Exception, ex:
                    print "Error logging message. Ex: {0}".format(str(ex))

        logger_thread = Thread(target=pull_logs)
        logger_thread.setDaemon(True)
        logger_thread.start()

        def pull_status():
            while True:
                status = self.__status_queue.get(block=True)
                try:
                    if self.__cmd_server is not None:
                        self.__cmd_server.send_message_to_all(
                            json.dumps(status))
                except Exception, ex:
                    print "Error delivering status. Ex: {0}".format(str(ex))

        status_thread = Thread(target=pull_status)
        status_thread.setDaemon(True)
        status_thread.start()

    def set_udp_stream(self, ip="127.0.0.1", port=1234):
        self.__udp_client = UdpSender(ip, port)
        return self

    def set_cmd_server(self, server):
        self.__cmd_server = server

    def critical(self, msg, tag="NO_TAG"):
        self.__log(LogTypes.CRITICAL, msg, tag)

    def warn(self, msg, tag="NO_TAG"):
        self.__log(LogTypes.WARNING, msg, tag)

    def debug(self, msg, tag="NO_TAG"):
        self.__log(LogTypes.DEBUG, msg, tag)

    def info(self, msg, tag="NO_TAG"):
        self.__log(LogTypes.INFO, msg, tag)

    def state(self, state_id, state_description, tag):
        self.info("Current state is '{0}'".format(state_description), tag)
        self.__status_queue.put(
            {"type": str(StatusTypes.STATE), "body": state_id})

    def __log(self, type, msg, tag):
        now = datetime.datetime.time(datetime.datetime.now())
        full_msg = "[{0}] [{1}] [{2}]: {3}".format(
            now, tag, str(type).rsplit('.', 1)[1], msg)
        self.__log_queue.put({"type": str(type), "body": full_msg})

    def __del__(self):
        self.__file.close()


Logger = _Logger()
