#include <random>		//for std::random_device

#include "vrPipe.h"
#include "vrSocketUtil.h"

vrPipe::vrPipe()
{
}

vrPipe::~vrPipe()
{
}

bool vrPipe::vr_pipe_create()
{
#if defined(_WIN32) || defined(WIN32)
	vrTcpSocket rp(socket(AF_INET, SOCK_STREAM, 0));
	vrTcpSocket wp(socket(AF_INET, SOCK_STREAM, 0));
	std::random_device rd;

	_pipefd[0] = rp.vr_tcp_socket_fd();
	_pipefd[1] = wp.vr_tcp_socket_fd();
	uint16_t port = 0;
	int nt = 5;

	while (nt--)
	{
		port = rd();//产生随机数
		if (rp.vr_tcp_socket_bind("127.0.0.1", port))
			break;
	}
	if (nt == 0)
		return false;

	if (!rp.vr_tcp_socket_listen(1))
		return false;

	if (!wp.vr_tcp_socket_connect("127.0.0.1", port))
		return false;

	if ((_pipefd[0] = rp.vr_tcp_socket_accept()) < 0)
		return false;

	vrSocketUtil::vr_socket_util_set_nonblock(_pipefd[0]);
	vrSocketUtil::vr_socket_util_set_nonblock(_pipefd[1]);

#elif defined(__linux) || defined(__linux__)
	if (pipe2(_pipefd, O_NONBLOCK | O_CLOEXEC) < 0)
	{
		return false;
	}
#endif
	return true;
}

int vrPipe::vr_pipe_write(void* buf, int len)
{
#if defined(_WIN32) || defined(WIN32)
	return send(_pipefd[1], (const char*)buf, len, 0);
#elif defined (__linux) || defined(__linux__)
	return write(_pipefd[1], buf, len);
#endif
}

int vrPipe::vr_pipe_read(void* buf, int len)
{
#if defined(_WIN32) || defined(WIN32)
	return recv(_pipefd[0], (char*)buf, len, 0);
#elif defined(__linux) || defined(__linux__)
	return read(_pipefd[0], buf, len);
#endif
}

void vrPipe::vr_pipe_close()
{
#if defined(_WIN32) || defined(WIN32)
	closesocket(_pipefd[0]);
	closesocket(_pipefd[1]);
#elif defined(__linux) || defined(__linux__)
	close(_pipefd[0]);
	close(_pipefd[1]);
#endif
}
int vrPipe::vr_pipe_readfd()
{
	return _pipefd[0];
}
int vrPipe::vr_pipe_writefd()
{
	return _pipefd[1];
}