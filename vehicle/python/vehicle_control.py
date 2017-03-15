from dronekit import VehicleMode, LocationGlobal
from time import time, sleep
from pymavlink import mavutil
import math
from common.python.logger import Logger
from collections import namedtuple
from numpy import sign
import datetime

AIR_SPEED = 3
GROUND_SPEED = 3
DEFAULT_TIMEOUT = 5
DEFAULT_GAP = 0.5
DISTANCE_FROM_TARGET_THRESHOLD = 2
MINIMUM_ALTITUDE_FACTOR = 0.95
MAXIMUM_ALTITUDE_FACTOR = 1.15
LANDING_ALTITUDE = 1
ABORT_ATTEMPTS = 3
VEHICLE_MODES = ["GUIDED", "LAND", "STABILIZE", "RTL", "LOITER", "POSHOLD", "ALT_HOLD"]

_Operation = namedtuple('Operation', 'predicate name timeout gap')
_Operation.__new__.__defaults__ = (None, "EMPTY", DEFAULT_TIMEOUT, DEFAULT_GAP)

TAG = "VehicleControl"

class _VehicleControl:
    def __init__(self, vehicle):
        self.vehicle = vehicle
        self.vehicle.airspeed = AIR_SPEED
        self.vehicle.ground_speed = GROUND_SPEED

    def get_altitude(self):
        rangefinder_finder = self.vehicle.rangefinder.distance
        if rangefinder_finder:
            roll = math.fabs(self.get_roll_angle())
            pitch = math.fabs(self.get_pitch_angle())
            a = math.cos(roll) * rangefinder_finder
            h = math.cos(pitch) * a
            return math.fabs(h)
        else:
            return self.vehicle.location.global_relative_frame.alt

    def get_roll_angle(self):
        return self.vehicle.attitude.roll

    def get_pitch_angle(self):
        return self.vehicle.attitude.pitch

    def goto_xyz(self, x, y, z):
        distance = math.sqrt(x * x + y * y + z * z)
        velocity = 1 if distance >= 100 else 0.1
        velocity_x = x / distance * velocity
        velocity_y = y / distance * velocity
        velocity_z = z / distance * velocity

        now = datetime.datetime.now()
        while datetime.datetime.now() < now + datetime.timedelta(milliseconds=distance / (float(velocity) / 10)):
            self.send_ned_velocity(velocity_y, velocity_x , -velocity_z)
            sleep(0.1)
        self.send_ned_velocity(0, 0 ,0)

    def goto_xy(self, x, y):
        self.goto_xyz(x, y, 0)

    def abort(self):
        Logger.info("Got abort signal. Trying to land vehicle", TAG)
        for i in range(ABORT_ATTEMPTS):
            if self.land():
                Logger.info("Abort succeeded", TAG)
                return

        Logger.critical("Abort Failed! You should take control from the RC!", TAG)

    def set_to_guided(self):
        return self.__set_mode("GUIDED")

    def arm(self):
        if self.vehicle.armed:
            return True

        operation = _Operation(predicate=lambda: self.vehicle.is_armable,
                               name="Check if vehicle is armable")

        if not self.__check_guided():
            Logger.warn("Operation '{0}' failed. Vehicle is not in guided mode".format(operation.name), TAG)
            return False

        if self.__run_in_time_window(operation):
            self.vehicle.armed = True
            operation = _Operation(predicate=lambda : self.vehicle.armed,
                                   name="Arm vehicle")
            return self.__run_in_time_window(operation)

        return False

    def disarm(self):
        self.vehicle.armed = False

    def land(self):
        operation = _Operation(predicate=lambda : self.vehicle.location.global_relative_frame.alt <= LANDING_ALTITUDE,
                               name="Land",
                               timeout=60)

        if not self.__check_armed_and_guided():
            Logger.warn("Operation '{0}' failed. Vehicle is not armed and guided".format(operation.name), TAG)
            return False

        self.__set_mode("LAND")
        return self.__run_in_time_window(operation)

    def takeoff(self, target_altitude):
        operation = _Operation(predicate=lambda :  self.get_altitude() >= target_altitude * MINIMUM_ALTITUDE_FACTOR,
                               name="Change altitude to: {0}".format(str(target_altitude)),
                               timeout=20)
        if not self.__check_armed_and_guided():
            Logger.warn("Operation '{0}' failed. Vehicle is not armed and guided".format(operation.name), TAG)
            return False

        self.vehicle.simple_takeoff(target_altitude)
        return self.__run_in_time_window(operation)

    # @param location of type LocationGlobal
    def goto_position(self, location):
        targetDistance = self.__get_distance_metres(self.vehicle.location.global_relative_frame, location)

        def to_run():
            remaining_distance = self.__get_distance_metres(self.vehicle.location.global_relative_frame, location)
            Logger.debug("Distance to target: {0}".format(str(remaining_distance)), TAG)
            return remaining_distance <= targetDistance * 0.01 or remaining_distance <= DISTANCE_FROM_TARGET_THRESHOLD

        operation = _Operation(predicate=to_run,
                               name="Go to position: {0}".format(str(location)),
                               timeout=100)

        '''if not self.__check_armed_and_guided():
            Logger.warn("Operation '{0}' failed. Vehicle is not armed and guided".format(operation.name), TAG)
            return False'''

        self.__goto_position_target_global_int(location)
        return self.__run_in_time_window(operation)

    def __del__(self):
        self.vehicle.close()

    def __set_mode(self, mode):
        operation = _Operation(predicate=lambda : self.vehicle.mode == VehicleMode(mode),
                               name="Set vechicle mode to '{0}'".format(mode))

        if mode not in VEHICLE_MODES:
            Logger.warn("Operation '{0}' failed. Unknown mode '{1}'".format(operation.name, mode), TAG)
            return False

        self.vehicle.mode = VehicleMode(mode)
        return self.__run_in_time_window(operation)

    def __check_armed_and_guided(self):
        if not self.__check_guided():
            return False

        if not self.vehicle.armed:
            return False

        return True

    def __check_guided(self):
        if self.vehicle.mode != VehicleMode("GUIDED"):
            return False
        else:
            return True

    def __run_in_time_window(self, operation):
        Logger.debug("Starting to execute drone operation '{0}' with timeout '{1}'".format(operation.name, operation.timeout), TAG)
        now = time()
        while time() < now + operation.timeout:
            if operation.predicate():
                Logger.debug("Operation '{0}' succeed".format(operation.name), TAG)
                return True
            sleep(operation.gap)

        Logger.warn("Operation '{0}' failed. Timeout reached.".format(operation.name), TAG)
        return False

    #region methods form dronekit
    # @param aLocation of type LocationGlobal
    def __goto_position_target_global_int(self, aLocation):
        """
        Send SET_POSITION_TARGET_GLOBAL_INT command to request the vehicle fly to a specified LocationGlobal.

        For more information see: https://pixhawk.ethz.ch/mavlink/#SET_POSITION_TARGET_GLOBAL_INT

        See the above link for information on the type_mask (0=enable, 1=ignore).
        At time of writing, acceleration and yaw bits are ignored.
        """
        msg = self.vehicle.message_factory.set_position_target_global_int_encode(
            0,  # time_boot_ms (not used)
            0, 0,  # target system, target component
            mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT_INT,  # frame
            0b0000111111111000,  # type_mask (only speeds enabled)
            aLocation.lat * 1e7,  # lat_int - X Position in WGS84 frame in 1e7 * meters
            aLocation.lon * 1e7,  # lon_int - Y Position in WGS84 frame in 1e7 * meters
            aLocation.alt,
            # alt - Altitude in meters in AMSL altitude, not WGS84 if absolute or relative, above terrain if GLOBAL_TERRAIN_ALT_INT
            0,  # X velocity in NED frame in m/s
            0,  # Y velocity in NED frame in m/s
            0,  # Z velocity in NED frame in m/s
            0, 0, 0,  # afx, afy, afz acceleration (not supported yet, ignored in GCS_Mavlink)
            0, 0)  # yaw, yaw_rate (not supported yet, ignored in GCS_Mavlink)
        # send command to vehicle
        self.vehicle.send_mavlink(msg)

    def get_distance_metres(self, aLocation1, aLocation2):
        """
        Returns the ground distance in metres between two LocationGlobal objects.

        This method is an approximation, and will not be accurate over large distances and close to the
        earth's poles. It comes from the ArduPilot test code:
        https://github.com/diydrones/ardupilot/blob/master/Tools/autotest/common.py
        """
        dlat = aLocation2.lat - aLocation1.lat
        dlong = aLocation2.lon - aLocation1.lon
        return math.sqrt((dlat * dlat) + (dlong * dlong)) * 1.113195e5

        #endregion

    def send_ned_velocity(self, velocity_x, velocity_y, velocity_z=0):
        """
        Move vehicle in direction based on specified velocity vectors.
        """
        msg = self.vehicle.message_factory.set_position_target_local_ned_encode(
            0,  # time_boot_ms (not used)
            0, 0,  # target system, target component
            mavutil.mavlink.MAV_FRAME_BODY_NED,  # frame
            0b0000111111000111,  # type_mask (only speeds enabled)
            0, 0, 0,  # x, y, z positions (not used)
            velocity_x, velocity_y, velocity_z,  # x, y, z velocity in m/s
            0, 0, 0,  # x, y, z acceleration (not supported yet, ignored in GCS_Mavlink)
            0, 0)  # yaw, yaw_rate (not supported yet, ignored in GCS_Mavlink)

        self.vehicle.send_mavlink(msg)


VehicleControl = None

def get():
    global VehicleControl
    return VehicleControl

def init(vehicle):
    global VehicleControl
    if not VehicleControl:
        VehicleControl = _VehicleControl(vehicle)
