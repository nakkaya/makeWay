objects = build/util.o build/NetworkInterface.o build/ArpFactory.o build/makeWay.o
CC = g++ -Wall

darwin : clean util  NetworkInterfaceDarwin ArpFactory driverDarwin
	$(CC) -lpcap $(LDFLAGS) $(objects) -o build/makeWay
linux : clean util NetworkInterfaceLinux ArpFactory driverLinux
	$(CC) -lpcap $(LDFLAGS) $(objects) -o build/makeWay

driverDarwin: makeWay.cpp
	$(CC) $(CXXFLAGS) -DOS_DARWIN  -c makeWay.cpp -o build/makeWay.o
driverLinux: makeWay.cpp
	$(CC) $(CXXFLAGS) -DOS_LINUX -c makeWay.cpp -o build/makeWay.o

NetworkInterfaceDarwin: NetworkInterface.cpp
	$(CC) $(CXXFLAGS) -DOS_DARWIN -c NetworkInterface.cpp -o build/NetworkInterface.o
NetworkInterfaceLinux: NetworkInterface.cpp
	$(CC) $(CXXFLAGS) -DOS_LINUX -c NetworkInterface.cpp -o build/NetworkInterface.o

util: util.cpp
	$(CC) $(CXXFLAGS) -c util.cpp -o build/util.o

ArpFactory: ArpFactory.cpp
	$(CC) $(CXXFLAGS) -c ArpFactory.cpp -o build/ArpFactory.o

clean:
	rm -rf build/ && mkdir build
