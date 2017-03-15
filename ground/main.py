from common.python.cmd_client import CMDClient
from common.python.udp_receiver import UdpReceiver
from communication_channel import CommunicationChannel
import argparse
def parse_arguments():
    parser = argparse.ArgumentParser(description='CommunicationChannel between the drone and ground controll')
    parser.add_argument('vehicle_ip', help="Mention the Drone ip to get logs and to be able to display information in the browser")
    return parser.parse_args()

if __name__=="__main__":
    args = parse_arguments()
    cmd_client = CMDClient(args.vehicle_ip)
    cmd_client.connect(True)
    udp_receiver = UdpReceiver()
    proxy = CommunicationChannel(cmd_client=cmd_client, udp_receiver=udp_receiver)
    proxy.start()
