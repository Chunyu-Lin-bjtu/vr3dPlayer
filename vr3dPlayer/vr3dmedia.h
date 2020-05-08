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
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "libavutil/pixdesc.h"
};

// 引入OpenCV库 视频渲染
#include <opencv2/opencv.hpp>

typedef enum {

	vr3dMovieErrorNone,
	vr3dMovieErrorOpenFile,
	vr3dMovieErrorStreamInfoNotFound,
	vr3dMovieErrorStreamNotFound,
	vr3dMovieErrorCodecNotFound,
	vr3dMovieErrorOpenCodec,
	vr3dMovieErrorAllocateFrame,
	vr3dMovieErroSetupScaler,
	vr3dMovieErroReSampler,
	vr3dMovieErroUnsupported,

} vr3dMovieError;

class vr3dmedia
{
public:
	vr3dmedia();
	~vr3dmedia();

public:
	virtual bool vr_3d_media_open_file(const char* path, int w, int h, int* perror = nullptr) = 0;

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
	virtual void vr_3d_media_init_rgb24();
	virtual double vr_3d_media_get_frame_count();
	virtual double vr_3d_media_get_fps();
	virtual bool vr_3d_media_get_next_frame(unsigned char* buf);

private:

};

class vr3dmedia_opencv : public vr3dmedia
{
public:
	vr3dmedia_opencv();
	~vr3dmedia_opencv();

public:
	virtual bool vr_3d_media_open_file(const char* path, int w, int h, int* perror = nullptr);
	virtual void vr_3d_media_init_rgb24();
	virtual double vr_3d_media_get_frame_count();
	virtual double vr_3d_media_get_fps();
	virtual bool vr_3d_media_get_next_frame(unsigned char* buf);
private:
	cv::VideoCapture _cap;
	int resize_width, resize_height;
	int frame_size;
};




class vr3dmoviedecoder
{
public:
	vr3dmoviedecoder();
	~vr3dmoviedecoder();

public:
	// 打开路径指定的媒体文件，perror返回错误码
	bool vr_3d_movie_decoder_open_file(const char* path, int* perror);

	// 初始化ffmpeg context，打开path指定的媒体文件
	vr3dMovieError vr_3d_movie_decoder_open_input(const char* path);

	vr3dMovieError vr_3d_movie_decoder_open_video_stream_by_idx(unsigned int idx);
	// 
	vr3dMovieError vr_3d_movie_decoder_open_video_stream();

public:// static 
	
	// 查找指定媒体类型的数据
	static vector<unsigned int> vr_3d_movie_decoder_collect_streams_idx(AVFormatContext* fmt_ctx, AVMediaType codec_type);

private:
	char* _path;


	AVCodecContext*  _video_codec_ctx;
	AVFormatContext* _fmt_ctx;
	AVFrame* _video_frame;
	int      _video_stream_idx;

	vector<unsigned int> _video_streams_idx;



	float _sampleRate;
	int   _frameWidth;
	int   _frameHeight;
};

#endif // !_VR_3D_MOVIE_DECODER_H_

