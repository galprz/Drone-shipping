TARGET = libvehicle
PYTHON_VERSION = 2.7
OS = $(shell uname)

USER_LOCAL_LIB = /usr/local/lib

ifeq ($(OS),Darwin)
	PYTHON_INCLUDE = /usr/local/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/Headers/
	PYTHON_LIB = /usr/local/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/lib
	BOOST_LIB =  /usr/local/opt/boost/lib
	BOOST_INCLUDE = /usr/include
	BOOST_PYTHON_INCLUDE = /usr/local/include
	BOOST_PYTHON_LIB = /usr/local/opt/boost

else
	PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
	PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)
	BOOST_LIB =  /usr/lib/x86_64-linux-gnu -lboost_system
	BOOST_INCLUDE = /usr/include
	BOOST_PYTHON_INCLUDE = /usr/include
	BOOST_PYTHON_LIB = /usr/lib

	SONAME = -soname,$(TARGET)
endif

OPENCV_INCLUDE = `pkg-config --cflags opencv`
OPENCV_LIB = `pkg-config --libs opencv`

COMPILE_FLAGS =  -O -std=c++11 -I$(PYTHON_INCLUDE) -I$(BOOST_PYTHON_INCLUDE) -fPIC $(OPENCV_INCLUDE) -I../../../common/cpp/src/ -I./common/ -I./mission/ -I./video/ -I./algorithm/ -I$(BOOST_INCLUDE)
LIBRARY_FLAGS = -shared -Wl,$(SONAME) -L$(BOOST_PYTHON_LIB) -lboost_python -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION) $(OPENCV_LIB) -L$(USER_LOCAL_LIB) -lcommon -L$(BOOST_LIB) -lboost_system -llapack -lblas -larmadillo

BUILD_DIR = ../build

$(BUILD_DIR)/$(TARGET).so: $(BUILD_DIR)/logger.o $(BUILD_DIR)/demo_mission.o $(BUILD_DIR)/service_provider.o $(BUILD_DIR)/state_machine.o $(BUILD_DIR)/vehicle_module_exception.o  $(BUILD_DIR)/collection.o  $(BUILD_DIR)/resize.o $(BUILD_DIR)/rotate.o $(BUILD_DIR)/direction_vector.o $(BUILD_DIR)/gray_color.o $(BUILD_DIR)/target_center.o $(BUILD_DIR)/video.o $(BUILD_DIR)/video_streamer.o $(BUILD_DIR)/video_recorder.o $(BUILD_DIR)/image_streamer.o  $(BUILD_DIR)/video_provider.o $(BUILD_DIR)/image_algorithm.o $(BUILD_DIR)/coarse_scan_mission.o $(BUILD_DIR)/analyze_image_mission.o $(BUILD_DIR)/find_and_land_mission.o $(BUILD_DIR)/up_down_mission.o $(BUILD_DIR)/python_main.o
	g++ -o $(BUILD_DIR)/$(TARGET).so $(BUILD_DIR)/logger.o $(BUILD_DIR)/demo_mission.o $(BUILD_DIR)/service_provider.o $(BUILD_DIR)/state_machine.o $(BUILD_DIR)/vehicle_module_exception.o $(BUILD_DIR)/collection.o  $(BUILD_DIR)/resize.o $(BUILD_DIR)/rotate.o $(BUILD_DIR)/direction_vector.o $(BUILD_DIR)/gray_color.o $(BUILD_DIR)/target_center.o $(BUILD_DIR)/video.o $(BUILD_DIR)/video_provider.o  $(BUILD_DIR)/video_streamer.o $(BUILD_DIR)/video_recorder.o $(BUILD_DIR)/image_streamer.o $(BUILD_DIR)/image_algorithm.o $(BUILD_DIR)/coarse_scan_mission.o $(BUILD_DIR)/analyze_image_mission.o $(BUILD_DIR)/find_and_land_mission.o $(BUILD_DIR)/up_down_mission.o $(BUILD_DIR)/python_main.o  $(LIBRARY_FLAGS)

$(BUILD_DIR)/logger.o: common/logger.cpp common/logger.hpp
	g++ $(COMPILE_FLAGS) -c common/logger.cpp -o $(BUILD_DIR)/logger.o

$(BUILD_DIR)/demo_mission.o: mission/demo_mission.hpp mission/demo_mission.cpp
	g++ $(COMPILE_FLAGS) -c mission/demo_mission.cpp -o $(BUILD_DIR)/demo_mission.o

$(BUILD_DIR)/state_machine.o: mission/state_machine.cpp mission/state_machine.hpp
	g++ $(COMPILE_FLAGS) -c mission/state_machine.cpp -o $(BUILD_DIR)/state_machine.o

$(BUILD_DIR)/service_provider.o: common/service_provider.cpp common/service_provider.hpp
	g++ $(COMPILE_FLAGS) -c common/service_provider.cpp -o $(BUILD_DIR)/service_provider.o

$(BUILD_DIR)/python_main.o: python_main.cpp
	g++ $(COMPILE_FLAGS) -c python_main.cpp -o $(BUILD_DIR)/python_main.o

$(BUILD_DIR)/vehicle_module_exception.o: common/vehicle_module_exception.cpp common/vehicle_module_exception.hpp
	g++ $(COMPILE_FLAGS) -c common/vehicle_module_exception.cpp -o $(BUILD_DIR)/vehicle_module_exception.o

$(BUILD_DIR)/collection.o: video/modifiers/collection.cpp video/modifiers/collection.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/collection.cpp -o $(BUILD_DIR)/collection.o

$(BUILD_DIR)/resize.o: video/modifiers/transformation/resize.cpp video/modifiers/transformation/resize.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/transformation/resize.cpp -o $(BUILD_DIR)/resize.o

$(BUILD_DIR)/gray_color.o: video/modifiers/filter/gray_color.cpp video/modifiers/filter/gray_color.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/filter/gray_color.cpp -o $(BUILD_DIR)/gray_color.o

$(BUILD_DIR)/rotate.o: video/modifiers/transformation/rotate.cpp video/modifiers/transformation/rotate.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/transformation/rotate.cpp -o $(BUILD_DIR)/rotate.o

$(BUILD_DIR)/target_center.o: video/modifiers/mask/target_center.cpp video/modifiers/mask/target_center.hpp algorithm/image_algorithm.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/mask/target_center.cpp -o $(BUILD_DIR)/target_center.o

$(BUILD_DIR)/direction_vector.o: video/modifiers/mask/direction_vector.cpp video/modifiers/mask/direction_vector.hpp
	g++ $(COMPILE_FLAGS) -c video/modifiers/mask/direction_vector.cpp -o $(BUILD_DIR)/direction_vector.o

$(BUILD_DIR)/video.o: video/video.cpp video/video.hpp
	g++ $(COMPILE_FLAGS) -c video/video.cpp -o $(BUILD_DIR)/video.o

$(BUILD_DIR)/video_provider.o: video/video_provider.cpp video/video_provider.hpp video/video_provider_config.hpp video/video.hpp
	g++ $(COMPILE_FLAGS)  -c video/video_provider.cpp -o $(BUILD_DIR)/video_provider.o

$(BUILD_DIR)/video_streamer.o: video/video_streamer.cpp video/video_streamer.hpp video/video_streamer_config.hpp video/video_provider.hpp video/video_provider_config.hpp video/video.hpp
	g++ $(COMPILE_FLAGS) -c video/video_streamer.cpp -o $(BUILD_DIR)/video_streamer.o

$(BUILD_DIR)/video_recorder.o: video/video_recorder.cpp video/video_recorder.hpp video/video_provider.hpp
	g++ $(COMPILE_FLAGS) -c video/video_recorder.cpp -o $(BUILD_DIR)/video_recorder.o

$(BUILD_DIR)/image_streamer.o: video/image_streamer.cpp video/image_streamer.hpp video/image_streamer_config.hpp video/video.hpp
	g++ $(COMPILE_FLAGS) -c video/image_streamer.cpp -o $(BUILD_DIR)/image_streamer.o

$(BUILD_DIR)/image_algorithm.o: algorithm/image_algorithm.cpp algorithm/image_algorithm.hpp algorithm/Img.h algorithm/Imgfwd.h algorithm/ImgVectorizer.h
	g++ $(COMPILE_FLAGS) -c algorithm/image_algorithm.cpp -o $(BUILD_DIR)/image_algorithm.o

$(BUILD_DIR)/coarse_scan_mission.o: mission/coarse_scan_mission.hpp mission/coarse_scan_mission.cpp mission/state_machine.hpp common/vehicle_module_exception.hpp
	g++ $(COMPILE_FLAGS) -c mission/coarse_scan_mission.cpp -o $(BUILD_DIR)/coarse_scan_mission.o

$(BUILD_DIR)/find_and_land_mission.o: mission/find_and_land_mission.hpp mission/find_and_land_mission.cpp mission/state_machine.hpp
	g++ $(COMPILE_FLAGS) -c mission/find_and_land_mission.cpp -o $(BUILD_DIR)/find_and_land_mission.o

$(BUILD_DIR)/analyze_image_mission.o: mission/analyze_image_mission.hpp mission/analyze_image_mission.cpp mission/state_machine.hpp
	g++ $(COMPILE_FLAGS) -c mission/analyze_image_mission.cpp -o $(BUILD_DIR)/analyze_image_mission.o

$(BUILD_DIR)/up_down_mission.o: mission/up_down_mission.hpp mission/up_down_mission.cpp mission/state_machine.hpp common/vehicle_module_exception.hpp
	g++ $(COMPILE_FLAGS) -c mission/up_down_mission.cpp -o $(BUILD_DIR)/up_down_mission.o


clean:
	rm -f $(BUILD_DIR)/*.{o,so}

install:
	cp $(BUILD_DIR)/$(TARGET).so $(USER_LOCAL_LIB)/$(TARGET).so
