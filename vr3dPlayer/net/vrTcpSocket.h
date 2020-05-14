#ifndef _VR_TCP_SOCKET_H_

#include <string>
#include "vrSocket.h"

namespace vr
{
	class vrTcpSocket
	{
	public:
		vrTcpSocket(SOCKET sockfd = -1);
		virtual ~vrTcpSocket();

		//´´½¨sockfd
		SOCKET vr_tcp_socket_create();

		//
		bool vr_tcp_socket_bind(std::string ip, uint16_t port);

	private:
		SOCKET _sockfd;
	};
}
#endif // !_VR_TCP_SOCKET_H_

