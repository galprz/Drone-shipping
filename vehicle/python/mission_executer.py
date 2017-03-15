from common.python.logger import Logger
import vehicle_control
from threading import Lock

TAG = "MissionExecuter"

mission_lock = Lock()

# mission should have a 'start' method
def execute_mission(mission):
    if mission_lock.acquire(0):
        try:
            if not vehicle_control.get().arm():
                Logger.warn("Could not arm vehicle. Will not start mission", TAG)
            else:
                mission.start()
        except RuntimeError, ex:
            Logger.critical("Cpp module threw exception: {0}. Aborting!", TAG)
            vehicle_control.abort()
        finally:
            vehicle_control.get().disarm()
            mission_lock.release()
    else:
        Logger.warn("Already executing a mission", TAG)