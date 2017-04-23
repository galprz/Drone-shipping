<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/logo.png" width="600"/>
</p>

[![Last build check](https://img.shields.io/badge/build-pass%20--%20last%20check%2003%2F17-green.svg)]()
[![Platform](https://img.shields.io/badge/platfrom-%20ubuntu%20%7C%20mac%20-lightgrey.svg)]()

## What it is

This is an open source project that intended to help people to create autonomous drone missions that operate with a [pixhawk](https://pixhawk.org/) controller.
The project is written in C++ and Python in order to enable fast image processing and operating the drone in real time.
The project also includes built in missions. Our goal was to fly to a specific GPS location, scan the area for a bullseye target and land on the center of the target.
You can use the framework to create your own missions.
The framework includes an API that helps to stream live video over wifi or/and record the video to file.<br>

We’ve created this project in the [Geomatric Image Processing lab at the Technion](http://gip.cs.technion.ac.il/). Our goal was to create a simple framework to manage simple and complex missions represented by state machine

## Requirements

1. g++
    [install g++ for mac](http://www-scf.usc.edu/~csci104/20142/installation/gccmac.html)
    or install for ubuntu : apt-get install g++
2. [boost](http://www.boost.org/)  you can install it with apt-get in linux or with brew in mac.
3. [boost-python](http://www.boost.org/doc/libs/1_63_0/libs/python/doc/html/index.html) - to link cpp with python [check that github project for examples](https://github.com/TNG/boost-python-examples)
4. Opencv cpp - for image processing
    [you don’t need the opencv for Python, because the image processing is done in the C++ code]
    you can install opencv on mac with 'port' or with brew or install with apt-get in linux.
5. [Armadillo](http://arma.sourceforge.net/) you can install it with apt-get in linux or with brew in mac.
6. [LAPACK — Linear Algebra PACKage](http://www.netlib.org/lapack/) you can install it with apt-get in linux or with brew in mac.
7. [BLAS - Basic Linear Algebra Subprograms](http://www.netlib.org/blas/) if you installed lapack you suppose to have blas check under /usr/lib
8. python
    [install python form mac](https://www.python.org/downloads/mac-osx/)
    or install for ubuntu : apt-get install python
9. pip - [python package manager](https://pip.pypa.io/en/stable/installing/)
10. python packages : 'websocket-client', 'websocket-server', 'enum34', 'dronekit', 'dronekit-sitl', 'numpy' run ```pip install -r requirements.txt``` in to project_root to install all
11. [nodeJs](https://nodejs.org/en/) + [http-server](https://www.npmjs.com/package/http-server) or any other way to simply create a localhost html server for displaying the UI

## Build

You can build the project by running ```sudo make``` in the root directory

## Components & Features

We divided the system to 3 main components:

1. vehicle - all the code that runs on the drone
2. ground - send commands to the drone and helps manage drone's missions.
3. common - common code that useful for both ground and vehicle

### Vehicle

If the installation process went smoothly you should have dronekit-sitl already installed.<br>
[dronekit-sitl](http://python.dronekit.io/develop/sitl_setup.html) is a program that simulates the pixhawk controller.
you can test it by running ``` dronekit-sitl copter ``` in the terminal, and it should look like this:
<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/dronekit-sitl.jpg" width="600"/>
</p>

You can see that the simulator waiting for connection on port 5760 .<br>
now you can run the vehicle program.<br>
just run that in the project root directory:
```
python -m vehicle.python.main tcp:<ip where you run dronekit-sitl>:5760 <ip where you want to see the live video stream>
```
(we will explain later how to see the video stream)

For example if you run the code in the same pc that you run dronekit-sitl you can just run:

```
python -m vehicle.python.main tcp:127.0.0.1:5760 127.0.0.1
```
You should see something like that if you don't have camera connected:
<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/vehicle-connected-without-camera.jpg" width="600"/>
</p>

Or something like that if you do :
<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/vehicle-connected-with-camera.jpg" width="600"/>
</p>

<b>Working with a real pixhawk instead of a simulator:</b><br>
If you working with Pixhawk and you have Odroid computer (or any other linux machine) that connects to it
You can run that line to connect the computer to the pixhawk and start send it commands.

```
python -m vehicle.python.main /dev/ttyUSB0,115200 <ground_ip>
```

### Ground

#### Communication

Now we need to create a communication between the vehicle(the drone) and the ground(the browser)
Run this code to establish the communication channel

```
python -m ground.main  <drone_ip>

```

For example if you run all in the same pc just run
```
python -m ground.main  127.0.0.1
```
It should look something like that :

<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/communication.jpg" width="600"/>
</p>

#### GUI - graphic user interface

We use html and javascript to display the state of the mission.

You can use http-server to display the GUI just follow those steps:

1. [Install nodeJs](https://nodejs.org/en/)
2. Installing nodeJs should install [npmJs](https://www.npmjs.com/) 'node package manager' use npm to install http-server by running ```npm install -g http-server``` in the terminal
3. navigate to droneproject/ground/ui and run http-server -p5555
4. naviagte to http://localhost:5555/ in chrome and open the [console](https://developers.google.com/web/tools/chrome-devtools/console/)
5. load the mission by running loadMission("FindAndLandMission")

It should Look like this when you run http-server from the terminal:

<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/http-server.jpg" width="600"/>
</p>

If you’ve already successfully established communication with the drone you are ready to connect, else just jump back to the [Communication](#communication) section.<br>
Connect the UI to the browser by following these steps:

1. Navigate to the [browser](http://localhost:5555) open the console and load the mission by running
loadMission("FindAndLandMission").
2. Connect to the drone by running client.connect().
3. Start the mission by running client.send('{"type" : "CMDTypes.FIND_AND_LAND_MISSION" , "body": {"alt":2, "distance": 100}}').

And after the mission start

<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/smwithcommands.jpg" width="600"/>
</p>

And then when you run the mission from the browser :

<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/state-machine.jpg" width="600"/>
</p>

**Note** : the 'Find and land' mission fits to our  specific drone. <br> the camera sits on the drone at specific rotation and we analyzing the image based on that, and control the drone to move in order to minimize the distance to the center of the target.

## How To Get Started

To better understand the architecture of that project start by reading the [project's presentation](#project-presentation) that explains about all of the components. <br>Then you can take a look at the missions that we’ve already created under project_root/vehicle/cpp/src/mission. (for example understand how 'Up and down' mission works).<br>
Finally you can try to create your own simple mission.

## Creating missions for the drone

The system provides an easy and convenient way to create missions for your drone.<br>
Every mission can be represented by a state machine. Every state in the state machine represents a state in the drone's mission.<br>
you can add mission by adding it to the mission folder under vehicle/cpp/src/mission/.
you can take a look on the UP_AND_DOWN_MISSION vehicle/cpp/src/mission/up_and_down_mission.hpp ,that is a simple mission with 2 states takeoff and land.<br>
Under ‘tools’ folder, you can find a script that will help you create mission. In order to use the tool, you need to create a json file (please refer to the examples in ‘ground/ui/Mission’).<br>
After creating the json file, run the script. The outputs are .hpp .cpp files that you can use. The only thing left for you is to implement the logic inside. Enjoy :)

## Project presentation

Under ```project_root/presentation```<br>
you can find our final project presentation, one version is a pdf and one is power point.

## Video

### Channels
We divid the video stream to channels.<br>
There are 2 channels in the system for now but you can add more.

<b>The DEFAULT channel:</b><br>
when you want to get the original video from the camera you should read the frame from the DEFAULT channel .
<br><br><b>The DEBUG channel:</b><br>
you can apply collection of modifiers to that channel . before the video provider provides a frame from that channel (or from any custom channel) it will change the frame according to the that collection of modifiers that applied for that channel.

<b>Create custom channel:</b><br>
You can create any custom channel very easily and the apply any collection of modifiers that you want like the DEBUG channel . take a look in the vide_provider.hpp for more info.

### Video stream
when you runs the drone code and get logs that every thing good with the camera and the video provider
started to get frame for it you can start the video_server that sits under the ground folder
run:
```
cd ground/video_server
./server 10000
```
And you should be able to see a live stream video from your drone
The video streamer sends the video stream from DEBUG channel so you would be able to apply
any masks filters and transformations on the video stream before you get it on the ground
see the scan state in the find_and_land_mission for example.

### Recording the video
The video recorder can record the video and save it under flights_video folder . <br>
In order to start recording you need to put the --record flag when you run the drone code .<br>
```
python -m vehicle.python.main tcp:127.0.0.1:5760 127.0.0.1 --record

```
The video recorder reads the video from the DEAFULT channel so he gets the original video without any modifications. if you want to modify the video before saving it to disk create RECORD channel and set
collection of modifiers to that channel and change the video recorder to read from that channel instead.

### Videos from our mission
Find and land mission's videos from the drone camera (without the go to GPS part)
After takeoff, the drone starts to scan the area and then decides where to go base on the image algorithm that tells him where is the center of the target.

**Click on the GIF to see the full video**:

[![Flight 1](https://j.gifs.com/O7D9OY.gif)](https://www.youtube.com/watch?v=e26DO6jsaFM&feature=youtu.be)


[![Flight 2](https://j.gifs.com/Y6mY9n.gif)](https://youtu.be/AuR8C7m42ZQ)

## Our Drone

<p align="center">
  <img src="https://raw.githubusercontent.com/galprz/DroneProject/master/images/our-drone.jpg" width="800"/>
</p>

## Useful links
* [Dronekit](http://python.dronekit.io/)
* [MavProxy](http://ardupilot.github.io/MAVProxy/html/index.html)
* [Mavlous](https://github.com/wiseman/mavelous)
* [APM planner](http://ardupilot.org/planner2/)
* [Mission planner](http://ardupilot.org/planner/index.html)
* [Calibrate the camera](http://docs.opencv.org/2.4/doc/tutorials/calib3d/camera_calibration/camera_calibration.html)
* [Calibrate the compose and the GPS](http://ardupilot.org/copter/docs/common-compass-calibration-in-mission-planner.html)
