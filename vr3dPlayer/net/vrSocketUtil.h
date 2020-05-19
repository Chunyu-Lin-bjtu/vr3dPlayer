#ifndef _VR_SOCKET_UTIL_H_
#define _VR_SOCKET_UTIL_H_
#include <string>
#include "vrSocket.h"


class vrSocketUtil
{
public:
	//��sockfd
	static bool vr_socket_util_bind(SOCKET sockfd, std::string ip, uint16_t port);

	//�����׽��ַ�����
	static void vr_socket_util_set_nonblock(SOCKET fd);

	//�����׽�������
	static void vr_socket_util_set_block(SOCKET fd, int writeTimeout = 0);

	//�����׽��ֵ�ַ����
	static void vr_socket_util_set_reuse_addr(SOCKET fd);

	//�����׽��ֶ˿�����
	static void vr_socket_util_set_reuse_port(SOCKET fd);

	//����Nagle�㷨����������С�����ͣ�Nagle�㷨����Ϊ�˾����ܷ��ʹ�����ݣ����������г�������С���ݿ�
	static void vr_socket_util_set_nodelay(SOCKET fd);

	//�����׽��ֱ������Ӽ��Է������Ƿ���������⣨����������Զ������TCP���ӵ�����
	static void vr_socket_util_set_keep_alive(SOCKET fd);

	//
	static void vr_socket_util_set_no_sigpipe(SOCKET fd);

	//�޸��׽��ַ��ͻ������Ĵ�С
	static void vr_socket_util_set_send_buf_size(SOCKET fd, int size);

	//�޸��׽��ֽ��ջ������Ĵ�С
	static void vr_socket_util_set_recv_buf_size(SOCKET fd, int size);

	//��ȡ�Զ�ip��ַ
	static std::string vr_socket_util_get_peer_ip(SOCKET fd);

	//��ȡ�Զ˵Ķ˿ں�
	static uint16_t vr_socket_util_get_peer_port(SOCKET fd);

	//��ȡ�Զ˵ĵ�ַ�ṹ��
	static int vr_socket_util_get_peer_addr(SOCKET fd, struct sockaddr_in* addr);

	//�ر��׽���
	static void vr_socket_util_close(SOCKET fd);

	//��������
	static bool vr_socket_util_connect(SOCKET fd, std::string ip, uint16_t port, int timeout);
private:

};

#endif // !_VR_SOCKET_UTIL_H_
