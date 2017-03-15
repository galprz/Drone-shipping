from enum import Enum

class CMDTypes(Enum):
    PING = 1
    GO_TO_GPS = 2
    ABORT_MISSION = 3
    DEMO_MISSION = 4
    COARSE_SCAN_MISSION = 5
    UP_DOWN_MISSION = 6
    SET_TO_GUIDED = 7
    ANALYZE_IMAGE_MISSION = 8
    FIND_AND_LAND_MISSION = 9

class StatusTypes(Enum):
    STATE = 1

class LogTypes(Enum):
    INFO = 1
    WARNING = 2
    CRITICAL = 3
    DEBUG = 4
