#include "vrTcpSocket.h"

#if defined(WIN32) || defined(_WIN32) 
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#endif 

namespace vr {
	vrTcpSocket::vrTcpSocket(SOCKET sockfd)
		: _sockfd(sockfd)
	{
	}

	vrTcpSocket::~vrTcpSocket()
	{
	}

	SOCKET vrTcpSocket::vr_tcp_socket_create()
	{
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		return _sockfd;
	}

	bool vrTcpSocket::vr_tcp_socket_bind(std::string ip, uint16_t port)
	{
		struct sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		addr.sin_port = htons(port);

		if (bind(_sockfd, (struct sockaddr*) & addr, sizeof(addr)) == SOCKET_ERROR)
		{

		}
		return false;
	}
}
