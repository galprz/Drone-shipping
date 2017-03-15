from common.logger import Logger
from threading import Thread
Logger.set_udp_stream()


def log1():
    while True:
        Logger.debug("test1")


def log2():
    while True:
        Logger.debug("test2")


t1 = Thread(target=log1)
t2 = Thread(target=log2)
t1.start()
t2.start()