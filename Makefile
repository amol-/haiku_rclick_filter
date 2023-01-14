rclick_ifilter.so: rclick_ifilter.cpp
	gcc -shared -o rclick_ifilter.so rclick_ifilter.cpp

install: rclick_ifilter.so
	cp rclick_ifilter.so /boot/home/config/non-packaged/add-ons/input_server/filters
	/system/servers/input_server -q

all: rclick_ifilter.so
