#ifndef _VR_SOCKET_UTIL_H_
#define _VR_SOCKET_UTIL_H_
#include <string>
#include "vrSocket.h"


class vrSocketUtil
{
public:
	//绑定sockfd
	static bool vr_socket_util_bind(SOCKET sockfd, std::string ip, uint16_t port);

	//设置套接字非阻塞
	static void vr_socket_util_set_nonblock(SOCKET fd);

	//设置套接字阻塞
	static void vr_socket_util_set_block(SOCKET fd, int writeTimeout = 0);

	//设置套接字地址复用
	static void vr_socket_util_set_reuse_addr(SOCKET fd);

	//设置套接字端口重用
	static void vr_socket_util_set_reuse_port(SOCKET fd);

	//禁用Nagle算法，允许数据小包发送，Nagle算法就是为了尽可能发送大块数据，避免网络中充斥着许多小数据块
	static void vr_socket_util_set_nodelay(SOCKET fd);

	//设置套接字保持连接检测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入
	static void vr_socket_util_set_keep_alive(SOCKET fd);

	//
	static void vr_socket_util_set_no_sigpipe(SOCKET fd);

	//修改套接字发送缓冲区的大小
	static void vr_socket_util_set_send_buf_size(SOCKET fd, int size);

	//修改套接字接收缓冲区的大小
	static void vr_socket_util_set_recv_buf_size(SOCKET fd, int size);

	//获取对端ip地址
	static std::string vr_socket_util_get_peer_ip(SOCKET fd);

	//获取对端的端口号
	static uint16_t vr_socket_util_get_peer_port(SOCKET fd);

	//获取对端的地址结构体
	static int vr_socket_util_get_peer_addr(SOCKET fd, struct sockaddr_in* addr);

	//关闭套接字
	static void vr_socket_util_close(SOCKET fd);

	//创建连接
	static bool vr_socket_util_connect(SOCKET fd, std::string ip, uint16_t port, int timeout);
private:

};

#endif // !_VR_SOCKET_UTIL_H_
