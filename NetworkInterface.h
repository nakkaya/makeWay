#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/sysctl.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "util.h"
#include <string.h>

#ifdef OS_DARWIN
#include <net/if_dl.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <net/route.h>
#endif

class NetworkInterface{
private:
  const char* dev;
public:
  NetworkInterface(const char* dv);
  std::string getLocalGateway();
  std::string getLocalIP();
  std::string getLocalMAC();
};
