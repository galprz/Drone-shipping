TARGET = libcommon
PYTHON_VERSION = 2.7
OS := $(shell uname)

ifeq ($(OS),Darwin)
	PYTHON_INCLUDE = /usr/local/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/Headers/
	PYTHON_LIB = /usr/local/Frameworks/Python.framework/Versions/$(PYTHON_VERSION)/lib
	BOOST_LIB = /usr/local/opt/boost/lib -lboost_system -lboost_thread-mt
	BOOST_INCLUDE = /usr/include
	BOOST_PYTHON_INCLUDE = /usr/local/include
	BOOST_PYTHON_LIB = /usr/local/opt/boost

	SONAME =
else
	PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
	PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)
	BOOST_LIB =  /usr/lib/x86_64-linux-gnu -lboost_system -lboost_thread
	BOOST_INCLUDE = /usr/include
	BOOST_PYTHON_INCLUDE = /usr/include
	BOOST_PYTHON_LIB = /usr/lib

	SONAME = -soname,$(TARGET)
endif


COMPILE_FLAGS = -O -std=c++11 -I$(PYTHON_INCLUDE) -I$(BOOST_PYTHON_INCLUDE) -fPIC -I$(BOOST_INCLUDE)
LIBRARY_FLAGS = -shared -Wl,$(SONAME) -L$(BOOST_PYTHON_LIB) -lboost_python -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION)  -L$(BOOST_LIB)

BUILD_DIR = ../build

$(BUILD_DIR)/$(TARGET).so: $(BUILD_DIR)/udp_sender.o $(BUILD_DIR)/udp_receiver.o $(BUILD_DIR)/clock.o $(BUILD_DIR)/gil_lock.o $(BUILD_DIR)/thread_locks.o $(BUILD_DIR)/rw_lock.o $(BUILD_DIR)/python_main.o
	g++ -o $(BUILD_DIR)/$(TARGET).so $(BUILD_DIR)/udp_sender.o $(BUILD_DIR)/udp_receiver.o $(BUILD_DIR)/clock.o $(BUILD_DIR)/rw_lock.o $(BUILD_DIR)/thread_locks.o $(BUILD_DIR)/gil_lock.o  $(BUILD_DIR)/python_main.o $(LIBRARY_FLAGS)

$(BUILD_DIR)/udp_sender.o: udp/udp_sender.cpp udp/udp_sender.hpp
	g++ $(COMPILE_FLAGS) -c udp/udp_sender.cpp -o $(BUILD_DIR)/udp_sender.o

$(BUILD_DIR)/udp_receiver.o: udp/udp_receiver.cpp udp/udp_receiver.hpp
	g++ $(COMPILE_FLAGS) -c udp/udp_receiver.cpp -o $(BUILD_DIR)/udp_receiver.o

$(BUILD_DIR)/clock.o: clock.cpp clock.hpp
	g++ $(COMPILE_FLAGS) -c clock.cpp -o $(BUILD_DIR)/clock.o

$(BUILD_DIR)/gil_lock.o: gil_lock.cpp gil_lock.hpp
	g++ $(COMPILE_FLAGS) -c gil_lock.cpp -o $(BUILD_DIR)/gil_lock.o

$(BUILD_DIR)/thread_locks.o: thread_locks.cpp thread_locks.hpp
	g++ $(COMPILE_FLAGS) -c thread_locks.cpp -o $(BUILD_DIR)/thread_locks.o

$(BUILD_DIR)/rw_lock.o: rw_lock.cpp rw_lock.hpp
	g++ $(COMPILE_FLAGS) -c rw_lock.cpp -o $(BUILD_DIR)/rw_lock.o

$(BUILD_DIR)/python_main.o: python_main.cpp
	g++ $(COMPILE_FLAGS) -c python_main.cpp -o $(BUILD_DIR)/python_main.o

clean:
	rm -f $(BUILD_DIR)/*.{o,so}

install:
	cp $(BUILD_DIR)/$(TARGET).so /usr/local/lib/$(TARGET).so
