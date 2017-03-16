drone:
	cd common/cpp/src && make && make install
	cd vehicle/cpp/src && make && make install
	cd ground/video_server/ && cmake . && make

clean:
	cd vehicle/cpp/src && make clean
	cd common/cpp/src && make clean
	cd ground/video_server/ && make clean && rm -rf CMakeCache.txt CMakeFiles
