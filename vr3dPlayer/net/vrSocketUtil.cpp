#include "vrLogger.h"
#include "vrSocketUtil.h"


bool vrSocketUtil::vr_socket_util_bind(SOCKET sockfd, std::string ip, uint16_t port)
{
	struct sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr*) & addr, sizeof(addr)) == SOCKET_ERROR)
	{
		LOG_DEBUG(" <socket=%d> bind <%s:%u> failed.\n", sockfd, ip.c_str(), port);
		return false;
	}

	return true;
}

void vrSocketUtil::vr_socket_util_set_nonblock(SOCKET fd)
{
#if defined (__linux) || defined(__linux__)
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	unsigned long on = 1;
	ioctlsocket(fd, FIONBIO, &on);
#endif
	
	return;
}

void vrSocketUtil::vr_socket_util_set_block(SOCKET fd, int writeTimeout)
{
#if defined(__linux) || defined(__linux__)
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags & (~O_NONBLOCK));
#elif defined(WIN32) || defined(_WIN32)
	unsigned long on = 0;
	ioctlsocket(fd, FIONBIO, &on);
#else
#endif

	if (writeTimeout > 0)
	{
#ifdef SO_SNDTIMEO
#if defined(__linux) || defined(__linux__) 
		struct timeval tv = { writeTimeout / 1000, (writeTimeout % 1000) * 1000 };
		setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof tv);
#elif defined(WIN32) || defined(_WIN32)
		unsigned long ms = (unsigned long)writeTimeout;
		setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&ms, sizeof(unsigned long));
#else
#endif
#endif
	}
}

void vrSocketUtil::vr_socket_util_set_reuse_addr(SOCKET fd)
{
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));
}

void vrSocketUtil::vr_socket_util_set_reuse_port(SOCKET fd)
{
#ifdef SO_REUSEPORT
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on));
#endif // SO_REUSEPORT
}

void vrSocketUtil::vr_socket_util_set_nodelay(SOCKET fd)
{
#ifdef TCP_NODELAY
	int on = 1;
	int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on));
#endif // TCP_NODELAY
}

void vrSocketUtil::vr_socket_util_set_keep_alive(SOCKET fd)
{
	int on = 1;
	int ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on));
}

void vrSocketUtil::vr_socket_util_set_no_sigpipe(SOCKET fd)
{
#ifdef SO_NOSIGPIPE
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (char*)&on, sizeof(on));
#endif
}

void vrSocketUtil::vr_socket_util_set_send_buf_size(SOCKET fd, int size)
{
	int ret = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

void vrSocketUtil::vr_socket_util_set_recv_buf_size(SOCKET fd, int size)
{
	int ret = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

std::string vrSocketUtil::vr_socket_util_get_peer_ip(SOCKET fd)
{
	struct sockaddr_in addr = { 0 };
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if (getpeername(fd, (struct sockaddr*) & addr, &addrlen) == 0) //getpeername函数用于获取与某个套接字关联的外地协议地址
	{
		return inet_ntoa(addr.sin_addr);
	}

	return "0.0.0.0";
}

uint16_t vrSocketUtil::vr_socket_util_get_peer_port(SOCKET fd)
{
	struct sockaddr_in addr = { 0 };
	socklen_t addrlen = sizeof(struct sockaddr_in);
	if (getpeername(fd, (struct sockaddr*) & addr, &addrlen) == 0)
	{
		return ntohs(addr.sin_port);
	}

	return 0;
}

int vrSocketUtil::vr_socket_util_get_peer_addr(SOCKET fd, sockaddr_in* addr)
{
	socklen_t addrlen = sizeof(struct sockaddr_in);
	return getpeername(fd, (struct sockaddr*)addr, &addrlen);
}

void vrSocketUtil::vr_socket_util_close(SOCKET fd)
{
#if defined(__linux) || defined(__linux__)
	close(fd);
#elif defined(WIN32) || defined(_WIN32)
	closesocket(fd);
#endif
}

bool vrSocketUtil::vr_socket_util_connect(SOCKET fd, std::string ip, uint16_t port, int timeout)
{
	bool connected = false;
	if (timeout > 0)
	{
		vrSocketUtil::vr_socket_util_set_nonblock(fd);
	}

	struct sockaddr_in addr = { 0 };
	socklen_t addrlen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	if (connect(fd, (struct sockaddr*) & addr, addrlen) == SOCKET_ERROR)
	{
		if (timeout > 0)
		{
			connected = false;
			fd_set fdWrite;
			FD_ZERO(&fdWrite);
			FD_SET(fd, &fdWrite);
			struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
			int n = select((int)fd + 1, nullptr, &fdWrite, nullptr, &tv);
			if (FD_ISSET(fd, &fdWrite))
			{
				connected = true;
			}
			vrSocketUtil::vr_socket_util_set_block(fd);
		}
		else
		{
			connected = false;
		}
	}
	return connected;
}
