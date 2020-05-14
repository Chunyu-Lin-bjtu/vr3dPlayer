#ifndef _VR_ENCODE_H_
#define _VR_ENCODE_H_
// ����GLEW�� ���徲̬����
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
	//���ļ���ȡһ֡ת��Ϊh264��ʽ
	bool vr_encode_h264_from_file(const char* src, const char* dst);
	void vr_encode_h264_run_file();

	//��framebufferֱ�Ӷ�ȡrgbת��Ϊh264��ʽ
	bool vr_encode_h264_from_framebuffer_direct(const char* dst);
	void vr_encode_h264_run_framebuffer_direct();

	//��framebufferӳ�䵽�ڴ棬��ȡӳ���ڴ�ת��Ϊh264��ʽ���ڴ�ӳ�䷽ʽ��ȡ֡�������ݣ������ʽ��1��pbo����˸��2���ȶ�
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

