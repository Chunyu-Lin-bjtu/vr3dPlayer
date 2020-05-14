#ifndef _VR_3D_MOVIE_DECODER_H_
#define _VR_3D_MOVIE_DECODER_H_
#include <list>
#include <map>
#include <mutex>
#include <vector>
#include <string>
using namespace std;

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include "libavutil/imgutils.h"
#include "libavutil/pixdesc.h"
};

// 引入OpenCV库 视频渲染
#include <opencv2/opencv.hpp>

typedef enum {

	vrMediaErrorNone,
	vrMediaErrorOpenFile,
	vrMediaErrorStreamInfoNotFound,
	vrMediaErrorStreamNotFound,
	vrMediaErrorCodecNotFound,
	vrMediaErrorOpenCodec,
	vrMediaErrorAllocateFrame,
	vrMediaErroSetupScaler,
	vrMediaErroReSampler,
	vrMediaErroUnsupported,

} vrMediaError;

class vr3dmedia
{
public:
	vr3dmedia();
	~vr3dmedia();

public:
	virtual bool vr_3d_media_open_file(const char* path, int w, int h, int* perror = nullptr) = 0;
	virtual bool vr_3d_media_open_screen(int w, int h, int* perror = nullptr) = 0;
	virtual void vr_3d_media_init_rgb24() = 0;

	virtual double vr_3d_media_get_frame_count() = 0;

	virtual double vr_3d_media_get_fps() = 0;

	virtual bool vr_3d_media_get_next_frame(unsigned char* buf) = 0;

protected:
	int frame_width, frame_height;

};


class vr3dmedia_ffmpeg : public vr3dmedia
{
public:
	vr3dmedia_ffmpeg();
	~vr3dmedia_ffmpeg();

public:
	virtual bool vr_3d_media_open_file(const char* path, int w, int h, int* perror = nullptr);
	virtual bool vr_3d_media_open_screen(int w, int h, int* perror = nullptr);
	virtual void vr_3d_media_init_rgb24();
	virtual double vr_3d_media_get_frame_count();
	virtual double vr_3d_media_get_fps();
	virtual bool vr_3d_media_get_next_frame(unsigned char* buf);

private:
	// 初始化ffmpeg context，打开path指定的媒体文件
	vrMediaError vr_media_ffmpeg_open_input(const char* path);
	// 
	vrMediaError vr_media_ffmpeg_open_video_stream();
	vrMediaError vr_media_ffmpeg_open_audio_stream();

	vrMediaError vr_media_ffmpeg_open_video_stream_by_idx();
	vrMediaError vr_media_ffmpeg_open_audio_stream_by_idx();

public:
	// 查找指定媒体类型的数据
	static unsigned int vr_media_ffmpeg_collect_streams_idx(AVFormatContext* fmt_ctx, AVMediaType codec_type);
private:
	AVFormatContext* fmt_ctx;
	AVStream* video_stream;
	AVCodecContext* video_codec_ctx;
	AVCodec* video_decoder;
	AVFrame* av_frame;
	AVFrame* gl_frame;
	AVPacket* packet;
	struct SwsContext* sws_ctx;
	int              video_streams_idx;
	int              audio_streams_idx;

	char* _path;

	AVStream* audio_stream;
	AVCodecContext* audio_codec_ctx;
	AVCodec* audio_decoder;

	int resize_width, resize_height;
	int frame_size;
};

class vr3dmedia_opencv : public vr3dmedia
{
public:
	vr3dmedia_opencv();
	~vr3dmedia_opencv();

public:
	virtual bool vr_3d_media_open_file(const char* path, int w, int h, int* perror = nullptr);
	virtual bool vr_3d_media_open_screen(int w, int h, int* perror = nullptr);
	virtual void vr_3d_media_init_rgb24();
	virtual double vr_3d_media_get_frame_count();
	virtual double vr_3d_media_get_fps();
	virtual bool vr_3d_media_get_next_frame(unsigned char* buf);
private:
	cv::VideoCapture _cap;
	int resize_width, resize_height;
	int frame_size;
};

#endif // !_VR_3D_MOVIE_DECODER_H_

