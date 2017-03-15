drone:
	cd common/cpp/src && make && make install
	cd vehicle/cpp/src && make && make install

clean:
	cd vehicle/cpp/src && make clean
	cd common/cpp/src && make clean