/*
 * NetWorkInfo.h
 *
 *  Created on: Mar 26, 2019
 *      Author: hdf_123
 */

#ifndef LTHUB_LTHUB_TERMINAL_CPP_UTILITY_NETWORKINFO_H_
#define LTHUB_LTHUB_TERMINAL_CPP_UTILITY_NETWORKINFO_H_

extern "C"
{
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/utsname.h>
	#include <arpa/inet.h>
	#include <errno.h>

	#include <fcntl.h>

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>

	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <string.h>

}

#include <string>

#include "Misc.h"

/******************************************************************************
DESCRIPTION:
	获取在和服务器在同一网段的ip地址
PARAMETERS:

RETURN:

******************************************************************************/
static string get_ip_network_segment(std::vector<std::string> &ip_addr_list, std::string service_ip)
{
	for(size_t i=0;i<ip_addr_list.size();++i)
	{
		vector<std::string> client_ip_segment;
		split_string(ip_addr_list[i], client_ip_segment, ".");

		vector<std::string> service_ip_segment;
		split_string(service_ip, service_ip_segment, ".");
		if(client_ip_segment[0]==service_ip_segment[0]&&
				client_ip_segment[1]==service_ip_segment[1]&&
				client_ip_segment[2]==service_ip_segment[2]
				)
		{
			return ip_addr_list[i];
		}
	}
	return ip_addr_list[0];
}


static inline void get_local_ip(int fd, std::vector<std::string> &ip_addr)
{
    struct ifreq ifr;
    char ip[32]={0};

    strcpy(ifr.ifr_name, "wlan0");
    ioctl(fd, SIOCGIFADDR, &ifr);
    strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    ip_addr.push_back(ip);

    strcpy(ifr.ifr_name, "eth0");
    ioctl(fd, SIOCGIFADDR, &ifr);
    strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    ip_addr.push_back(ip);

    return;
}

static inline void get_hostname(std::string & hostname)
{
    struct utsname name;
    uname(&name);
    hostname = name.nodename;
}

static inline void get_mac_address(int fd, std::string & mac_addr)
{

    char mac[32] = { 0 };
	int intrface = 0;
	struct ifreq buf[16];
	struct ifconf ifc;

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;

	if(ioctl(fd, SIOCGIFCONF, (char *) &ifc) < 0)
	{
		return;
	}

	intrface = ifc.ifc_len / sizeof (struct ifreq);
	while (intrface-- > 0)
	{
		if((strncmp(buf[intrface].ifr_name, "wlan0", 5) != 0) )
		{
			continue;
		}

		if ((ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface])) < 0)
		{
			continue;
		}

		if( !(buf[intrface].ifr_flags & IFF_UP) )
		{
			continue;
		}

		if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
		{
            sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x", buf[intrface].ifr_hwaddr.sa_data[0],buf[intrface].ifr_hwaddr.sa_data[1],
					buf[intrface].ifr_hwaddr.sa_data[2],buf[intrface].ifr_hwaddr.sa_data[3],buf[intrface].ifr_hwaddr.sa_data[4],
					buf[intrface].ifr_hwaddr.sa_data[5]);

            mac_addr = mac;
			break;
		}
	}

	return;
}

#endif /* LTHUB_LTHUB_TERMINAL_CPP_UTILITY_NETWORKINFO_H_ */
