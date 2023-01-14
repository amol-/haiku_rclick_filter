all:
	gcc -shared -o rclick_ifilter.so rclick_ifilter.cpp

install:
	cp rclick_ifilter.so /boot/home/config/non-packaged/add-ons/input_server/filters
	/system/servers/input_server -q
