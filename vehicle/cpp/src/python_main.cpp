//
//  python_main.cpp
//  Vehicle CPP Module
//
//  Created by Gal Malka on 29/11/2016.
//  Copyright © 2016 Gal Malka. All rights reserved.
//

#include <stdio.h>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/exception_translator.hpp>
#include <string>
#include "mission/demo_mission.hpp"
#include "mission/analyze_image_mission.hpp"
#include "mission/coarse_scan_mission.hpp"
#include "mission/find_and_land_mission.hpp"
#include "mission/up_down_mission.hpp"
#include "common/service_provider.hpp"
#include "video/video_provider.hpp"
#include "video/video_streamer.hpp"
#include "video/video_recorder.hpp"
#include "video/image_streamer.hpp"
#include "common/vehicle_module_exception.hpp"

using namespace boost;
using namespace VehicleModule::Common;
using namespace VehicleModule::Mission;
using namespace VehicleModule::Video;

void translate_vehicle_module_exception(const VehicleModuleException& ex) {
    PyErr_SetString(PyExc_RuntimeError, ex.what());
};

BOOST_PYTHON_MODULE(libvehicle)
{
    python::class_<DemoMission>("DemoMission", python::init<double, double, double>())
    .def("start", &DemoMission::start);

    python::class_<CoarseScanMission>("CoarseScanMission", python::init<double, double, double>())
    .def("start", &CoarseScanMission::start);

    python::class_<FindAndLandMission>("FindAndLandMission", python::init<double, double, double>())
    .def("start", &FindAndLandMission::start);

    python::class_<UpDownMission>("UpDownMission", python::init<double>())
    .def("start", &UpDownMission::start);

    python::class_<AnalyzeImageMission>("AnalyzeImageMission", python::init<>())
    .def("start", &AnalyzeImageMission::start);

    void (ServiceProvider::*publish_python)(const std::string& , PyObject* )  = &ServiceProvider::publish;

    python::class_<ServiceProvider, boost::noncopyable>("ServiceProvider", python::no_init)
    .def("publish", publish_python)
    .def("get_instance", &ServiceProvider::get_instance, python::return_value_policy<python::reference_existing_object>())
    .staticmethod("get_instance");

    python::class_<VideoProvider, boost::noncopyable>("VideoProvider", python::no_init)
    .def("start_listen_to_camera", &VideoProvider::start_listen_to_camera)
    .def("stop_listen_to_camera", &VideoProvider::stop_listen_to_camera)
    .def("get_instance", &VideoProvider::get_instance, python::return_value_policy<python::reference_existing_object>())
    .staticmethod("get_instance");

    python::class_<VideoStreamer, boost::noncopyable>("VideoStreamer", python::init<std::string, int >())
    .def("start_sending_video", &VideoStreamer::start_sending_video)
    .def("stop_sending_video", &VideoStreamer::stop_sending_video);

    python::class_<VideoRecorder, boost::noncopyable>("VideoRecorder", python::init<std::string>())
    .def("start_recording_video", &VideoRecorder::start_recording_video)
    .def("stop_recording_video", &VideoRecorder::stop_recording_video);

    python::class_<ImageStreamer, boost::noncopyable>("ImageStreamer", python::init<std::string, int >())
    .def("start_sending_images", &ImageStreamer::start_sending_images)
    .def("stop_sending_images", &ImageStreamer::stop_sending_images)
    .def("publish_as_cpp_service", &ImageStreamer::publish_as_cpp_service);

    python::register_exception_translator<VehicleModuleException>(&translate_vehicle_module_exception);
}
