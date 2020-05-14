#ifndef _VR_ENCODE_H_
#define _VR_ENCODE_H_
// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include "GL/glew.h"
#include <iostream>
using namespace std;

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
	//#include <sys/time.h>
}
class vrEncode
{
public:
	vrEncode();
	~vrEncode();

private:

};



class vrEncodeH264 : public vrEncode
{
public:
	vrEncodeH264(int w, int h, int fps);
	~vrEncodeH264();

public:
	//从文件读取一帧转换为h264格式
	bool vr_encode_h264_from_file(const char* src, const char* dst);
	void vr_encode_h264_run_file();

	//从framebuffer直接读取rgb转换为h264格式
	bool vr_encode_h264_from_framebuffer_direct(const char* dst);
	void vr_encode_h264_run_framebuffer_direct();

	//从framebuffer映射到内存，读取映射内存转换为h264格式，内存映射方式读取帧缓存数据，这个方式，1个pbo会闪烁，2个稳定
	bool vr_encode_h264_from_framebuffer_map(int n, const char* dst);
	void vr_encode_h264_run_framebuffer_map();


private:
	bool vr_encode_h264_init();

	bool vr_encode_h264_create_output_ctx(const char* dst);
	
	//
	bool vr_encode_h264_frame();
	void vr_encode_h264_set_option();

private:
	FILE* _ifp;
	int _width;
	int _height;
	int _fps;

	AVCodec* codec;
	AVCodecContext* c;
	AVFormatContext* oc;
	SwsContext* ctx;
	AVFrame* yuv;
	uint8_t* rgb;

	GLuint* _pbo;
	GLuint _n;
};

#endif // !_VR_ENCODE_H_

