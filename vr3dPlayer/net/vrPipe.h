#ifndef _VR_PIPE_H_
#define _VR_PIPE_H_

#include "vrTcpSocket.h"

class vrPipe
{
public:
	vrPipe();
	~vrPipe();

	//�����ܵ���Windowsƽ̨��tcp����ģ��
	bool vr_pipe_create();

	//���ܵ�д����
	int vr_pipe_write(void* buf, int len);

	//�ӹܵ�������
	int vr_pipe_read(void* buf, int len);

	//�رչܵ�
	void vr_pipe_close();

	//���عܵ�����id
	int vr_pipe_readfd();

	//���عܵ�д��id
	int vr_pipe_writefd();

private:
	SOCKET _pipefd[2];//_pipefd[0]���ˣ�_pipefd[1]д��
};

#endif // !_VR_PIPE_H_

