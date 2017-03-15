import argparse
import vehicle_control
from dronekit import connect, LocationGlobal,Vehicle
from threading import Thread
from time import sleep
import vehicle.cpp.build.libvehicle as libvehicle
from vehicle.cpp.build.libvehicle import ServiceProvider
from vehicle.cpp.build.libvehicle import DemoMission, CoarseScanMission, UpDownMission, AnalyzeImageMission, FindAndLandMission
from vehicle.cpp.build.libvehicle import VideoProvider, VideoStreamer,VideoRecorder
from common.python.logger import Logger
from common.python.cmd_server import CMDServer
from common.python.types import CMDTypes
from mission_executer import execute_mission
import time
import datetime
import os

TAG = "OdroidMain"

def parse_arguments():
    parser = argparse.ArgumentParser(description='HUD module')
    parser.add_argument('vehicle_connection_string', help="E.g. /dev/ttyACM0 or /dev/ttyUSB0,57600")
    parser.add_argument('gcs_ip', help="Mention the GCS ip to make a UDP connection with it")
    parser.add_argument('--record', help="Record the video from the camera", default=False, const=True, action='store_const')
    return parser.parse_args()

def connect_to_vehicle(vehical_connection_string):
    Logger.info("Connecting to vehicle...", TAG)
    vehicle = connect(vehical_connection_string, wait_ready=True)
    vehicle.wait_ready('autopilot_version')
    Logger.info("Autopilot Firmware version: %s" % vehicle.version, TAG)
    Logger.info("Major version number: %s" % vehicle.version.major, TAG)
    Logger.info("Minor version number: %s" % vehicle.version.minor, TAG)
    Logger.info("Patch version number: %s" % vehicle.version.patch, TAG)
    Logger.info("Release type: %s" % vehicle.version.release_type(), TAG)
    Logger.info("Release version: %s" % vehicle.version.release_version(), TAG)
    Logger.info("Stable release?: %s" % vehicle.version.is_stable(), TAG)
    return vehicle

def on_demo_command(args):
    mission = DemoMission(float(args["lat"]), float(args["lon"]), float(args["alt"]))
    mission_thread = Thread(target=execute_mission, args=[mission])
    mission_thread.setDaemon(True)
    mission_thread.start()

def on_coarse_scan_command(args):
    mission = CoarseScanMission(float(args["alt"]), float(args["distance"]), float(args["j"]))
    mission_thread = Thread(target=execute_mission, args=[mission])
    mission_thread.setDaemon(True)
    mission_thread.start()

def on_find_and_land_mission(args):
    mission = FindAndLandMission(float(args["alt"]), float(args["distance"]), float(args["j"]))
    mission_thread = Thread(target=execute_mission, args=[mission])
    mission_thread.setDaemon(True)
    mission_thread.start()

def on_analyze_image_command(args):
    mission = AnalyzeImageMission()
    mission_thread = Thread(target=execute_mission, args=[mission])
    mission_thread.setDaemon(True)
    mission_thread.start()

def on_up_down_command(args):
    mission = UpDownMission(float(args["alt"]))
    mission_thread = Thread(target=execute_mission, args=[mission])
    mission_thread.setDaemon(True)
    mission_thread.start()

def on_abort_command(args):
    Logger.info("Got abort command from user", TAG)
    vehicle_control.get().abort()

def on_ping(args):
    Logger.info("Got ping command from user", TAG)

def set_to_guided(args=None):
    if not vehicle_control.get().set_to_guided():
        Logger.warn("Could not set vehicle mode to guided. Will not start mission", TAG)

if __name__ == "__main__":
    args = parse_arguments()

    # Setup logger
    Logger.set_udp_stream(args.gcs_ip, 1234)

    # Connect to drone
    vehicle = connect_to_vehicle(args.vehicle_connection_string)
    vehicle_control.init(vehicle)

    ServiceProvider.get_instance().publish("logger", Logger);
    ServiceProvider.get_instance().publish("vehicle_control", vehicle_control.get());

    # Setup Video provider
    video_provider = VideoProvider.get_instance()
    video_provider_thread = Thread(target=video_provider.start_listen_to_camera)
    video_provider_thread.setDaemon(True)
    video_provider_thread.start()

    # Setup Video streamer
    video_streamer = VideoStreamer(args.gcs_ip,10000)
    video_streamer_thread = Thread(target=video_streamer.start_sending_video)
    video_streamer_thread.setDaemon(True)
    video_streamer_thread.start()

    # Setup Video recorder
    if args.record:
        if not os.path.exists("./flights_video/"):
            os.makedirs("./flights_video/")
        video_recorder = VideoRecorder("./flights_video/flight_" + str(datetime.datetime.now().strftime("%d-%m-%YT%H.%M.%S")) + ".avi")
        video_recorder_thread = Thread(target=video_recorder.start_recording_video)
        video_recorder_thread.setDaemon(True)
        video_recorder_thread.start()

    # Setup command server
    commands_server = CMDServer()
    commands_server.set_on_message_handler(CMDTypes.PING, on_ping)
    commands_server.set_on_message_handler(CMDTypes.DEMO_MISSION, on_demo_command)
    commands_server.set_on_message_handler(CMDTypes.COARSE_SCAN_MISSION, on_coarse_scan_command)
    commands_server.set_on_message_handler(CMDTypes.UP_DOWN_MISSION, on_up_down_command)
    commands_server.set_on_message_handler(CMDTypes.ABORT_MISSION, on_abort_command)
    commands_server.set_on_message_handler(CMDTypes.SET_TO_GUIDED, set_to_guided)
    commands_server.set_on_message_handler(CMDTypes.ANALYZE_IMAGE_MISSION, on_analyze_image_command)
    commands_server.set_on_message_handler(CMDTypes.FIND_AND_LAND_MISSION, on_find_and_land_mission)
    command_server_thread = Thread(target=commands_server.run_forever)
    command_server_thread.setDaemon(True)
    command_server_thread.start()

    Logger.set_cmd_server(commands_server)

    # Set vehicle to guided mode
    set_to_guided()

    while True:
        try:
            sleep(1)
        except KeyboardInterrupt:
            Logger.warn("Got KeyboardInterrupt. Aborting and exiting", TAG)
            vehicle_control.get().abort()
            if args.record:
                video_recorder.stop_recording_video()
                video_recorder_thread.join()
            video_provider.stop_listen_to_camera()
            video_provider_thread.join()
            video_streamer.stop_sending_video()
            video_streamer_thread.join()
            exit()
