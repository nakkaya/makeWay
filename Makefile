objects = build/util.o build/NetworkInterface.o build/ArpFactory.o build/makeWay.o
CC = g++ -Wall

darwin : clean util  NetworkInterfaceDarwin ArpFactory driverDarwin
	$(CC) -lpcap $(objects) -o build/makeWay
linux : clean util NetworkInterfaceLinux ArpFactory driverLinux
	$(CC) -lpcap $(objects) -o build/makeWay

driverDarwin: makeWay.cpp
	$(CC) -DOS_DARWIN  -c makeWay.cpp -o build/makeWay.o
driverLinux: makeWay.cpp
	$(CC) -DOS_LINUX -c makeWay.cpp -o build/makeWay.o

NetworkInterfaceDarwin: NetworkInterface.cpp
	$(CC) -DOS_DARWIN -c NetworkInterface.cpp -o build/NetworkInterface.o
NetworkInterfaceLinux: NetworkInterface.cpp
	$(CC) -DOS_LINUX -c NetworkInterface.cpp -o build/NetworkInterface.o

util: util.cpp
	$(CC)  -c util.cpp -o build/util.o

ArpFactory: ArpFactory.cpp
	$(CC)  -c ArpFactory.cpp -o build/ArpFactory.o

clean:
	rm -rf build/ && mkdir build
