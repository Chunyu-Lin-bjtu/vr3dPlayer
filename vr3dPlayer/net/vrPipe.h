#ifndef _VR_PIPE_H_
#define _VR_PIPE_H_

#include "vrTcpSocket.h"

class vrPipe
{
public:
	vrPipe();
	~vrPipe();

	//创建管道，Windows平台用tcp网络模拟
	bool vr_pipe_create();

	//往管道写数据
	int vr_pipe_write(void* buf, int len);

	//从管道读数据
	int vr_pipe_read(void* buf, int len);

	//关闭管道
	void vr_pipe_close();

	//返回管道读端id
	int vr_pipe_readfd();

	//返回管道写端id
	int vr_pipe_writefd();

private:
	SOCKET _pipefd[2];//_pipefd[0]读端，_pipefd[1]写端
};

#endif // !_VR_PIPE_H_

