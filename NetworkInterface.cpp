#include "NetworkInterface.h"
#include <cstdio>

NetworkInterface::NetworkInterface( const char* dv ){
  dev = dv;
}

#ifdef OS_DARWIN
std::string NetworkInterface::getLocalGateway( ){

  struct in_addr gatewayaddr;
  in_addr_t * addr = &(gatewayaddr.s_addr);

  //in_addr_t * addr;
  int mib[] = {CTL_NET, PF_ROUTE, 0, AF_INET,
	       NET_RT_FLAGS, RTF_GATEWAY};
  size_t l;
  char * buf, * p;
  struct rt_msghdr * rt;
  struct sockaddr * sa;
  struct sockaddr * sa_tab[RTAX_MAX];
  int i;
  if(sysctl(mib, sizeof(mib)/sizeof(int), 0, &l, 0, 0) < 0) {
    return std::string("Unknown");
  }

  if(l>0) {
    buf = (char*)malloc(l);
    if(sysctl(mib, sizeof(mib)/sizeof(int), buf, &l, 0, 0) < 0) {
      free(buf);
      return std::string("Unknown");
    }
    for(p=buf; p<buf+l; p+=rt->rtm_msglen) {
      rt = (struct rt_msghdr *)p;
      sa = (struct sockaddr *)(rt + 1);
      for(i=0; i<RTAX_MAX; i++) {
	if(rt->rtm_addrs & (1 << i)) {
	  sa_tab[i] = sa;
	  sa = (struct sockaddr *)((char *)sa + sa->sa_len);
	} else {
	  sa_tab[i] = NULL;
	}
      }
      if( ((rt->rtm_addrs & (RTA_DST|RTA_GATEWAY)) == (RTA_DST|RTA_GATEWAY))
	  && sa_tab[RTAX_DST]->sa_family == AF_INET
	  && sa_tab[RTAX_GATEWAY]->sa_family == AF_INET) {
	if(((struct sockaddr_in *)sa_tab[RTAX_DST])->sin_addr.s_addr == 0) {
	  *addr = ((struct sockaddr_in *)(sa_tab[RTAX_GATEWAY]))->sin_addr.s_addr;
	}
      }
    }

    free(buf);
  }

  return IPtoString((uint8_t*)addr);
}
#endif

#ifdef OS_LINUX
std::string NetworkInterface::getLocalGateway( ){
  struct in_addr gatewayaddr;
  in_addr_t * addr = &(gatewayaddr.s_addr);

  long d, g;
  char buf[256];
  int line = 0;
  FILE * f;
  char * p;
  f = fopen("/proc/net/route", "r");
  if(!f)
    return "Unknown";
  while(fgets(buf, sizeof(buf), f)) {
    if(line > 0) {
      p = buf;
      while(*p && !isspace(*p))
	p++;
      while(*p && isspace(*p))
	p++;
      if(sscanf(p, "%lx%lx", &d, &g)==2) {
	if(d == 0) { /* default */
	  *addr = g;
	  fclose(f);
	  return IPtoString((uint8_t*)addr);
	}
      }
    }
    line++;
  }
  /* default route not found ! */
  if(f)
    fclose(f);
  return "Unknown";
}
#endif


/* Given a ifreq structure this function returns its IP address */
std::string NetworkInterface::getLocalIP( ){
  
  struct ifreq ifr;
  char netaddr[INET_ADDRSTRLEN];
  int sd;
  sd=socket(AF_INET,SOCK_DGRAM,0);
  strcpy(ifr.ifr_name, dev );
  if((ioctl(sd,SIOCGIFADDR,(caddr_t)&ifr,sizeof(struct ifreq)))<0)
    perror("Error : ");


  struct sockaddr *sa;
  sa=(struct sockaddr *)&(ifr.ifr_addr);
  switch(sa->sa_family){
  case AF_INET6:
    inet_ntop(AF_INET6,(struct in6_addr *)&(((struct sockaddr_in6 *)sa)->sin6_addr),
	      netaddr,INET6_ADDRSTRLEN);
    break;
  default : strcpy(netaddr,inet_ntoa(((struct sockaddr_in *)sa)->sin_addr));
  }

  std::string ip(netaddr);  
  return ip;
}

#ifdef OS_DARWIN
std::string NetworkInterface::getLocalMAC(){
  int                     mib[6];
  size_t len;
  char                    *buf;
  unsigned char           *ptr;
  struct if_msghdr        *ifm;
  struct sockaddr_dl      *sdl; 

  mib[0] = CTL_NET;
  mib[1] = AF_ROUTE;
  mib[2] = 0;
  mib[3] = AF_LINK;
  mib[4] = NET_RT_IFLIST; 

  if ((mib[5] = if_nametoindex(dev)) == 0) {
    perror("if_nametoindex error");
    exit(2);
  } 

  if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
    perror("sysctl 1 error");
    exit(3);
  }

  if ((buf = (char *)malloc(len)) == NULL) {
    perror("malloc error");
    exit(4);
  }
  
  if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
    perror("sysctl 2 error");
    exit(5);
  } 

  ifm = (struct if_msghdr *)buf;
  sdl = (struct sockaddr_dl *)(ifm + 1);
  ptr = (unsigned char *)LLADDR(sdl);

  return MACtoString( ptr );
}
#endif

#ifdef OS_LINUX
std::string NetworkInterface::getLocalMAC(){

  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

  ioctl(fd, SIOCGIFHWADDR, &ifr);

  close(fd);

  return MACtoString((uint8_t*)ifr.ifr_hwaddr.sa_data);
}
#endif
