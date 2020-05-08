#include "vr3dmedia.h"

vr3dmedia::vr3dmedia()
{
}

vr3dmedia::~vr3dmedia()
{
}


vr3dmedia_ffmpeg::vr3dmedia_ffmpeg()
{
}

vr3dmedia_ffmpeg::~vr3dmedia_ffmpeg()
{
}

bool vr3dmedia_ffmpeg::vr_3d_media_open_file(const char* path, int w, int h, int* perror)
{
	return false;
}

void vr3dmedia_ffmpeg::vr_3d_media_init_rgb24()
{
}

double vr3dmedia_ffmpeg::vr_3d_media_get_frame_count()
{
	return 0.0;
}

double vr3dmedia_ffmpeg::vr_3d_media_get_fps()
{
	return 0.0;
}

bool vr3dmedia_ffmpeg::vr_3d_media_get_next_frame(unsigned char* buf)
{
	return false;
}


vr3dmedia_opencv::vr3dmedia_opencv()
{
}

vr3dmedia_opencv::~vr3dmedia_opencv()
{
}

bool vr3dmedia_opencv::vr_3d_media_open_file(const char* path, int w, int h, int* perror)
{
	_cap.open(path);
	//_cap.open(0); // 打开系统camera
	if (!_cap.isOpened()) {
		std::cout << "vr3dmedia_opencv:: can not open video. " << path << std::endl;
		return false;
	}

	resize_width = w;
	resize_height = h;

	frame_size = w * h * 3;
	return true;
}

void vr3dmedia_opencv::vr_3d_media_init_rgb24()
{
}

double vr3dmedia_opencv::vr_3d_media_get_frame_count()
{
	return _cap.get(CV_CAP_PROP_FRAME_COUNT);
}

double vr3dmedia_opencv::vr_3d_media_get_fps()
{
	return _cap.get(CV_CAP_PROP_FPS);
}

bool vr3dmedia_opencv::vr_3d_media_get_next_frame(unsigned char* buf)
{
	cv::Mat frame;
	_cap >> frame;
	if (!frame.data) {
		return false;
	}

	cv::resize(frame, frame, cv::Size(resize_width, resize_height), 0, 0, cv::INTER_NEAREST);
	cv::cvtColor(frame, frame, CV_BGR2RGB);

	std::memcpy(buf, frame.data, frame_size);
	if (cv::waitKey(50) >= 0) {
		return false;
	}

	return true;
	
}


vr3dmoviedecoder::vr3dmoviedecoder()
{
}

vr3dmoviedecoder::~vr3dmoviedecoder()
{
}

bool vr3dmoviedecoder::vr_3d_movie_decoder_open_file(const char* path, int* perror)
{
	vr3dMovieError err = vr3dMovieErrorNone;
	
	_path = (char*)path;
	err = vr_3d_movie_decoder_open_input(path);
	if (err == vr3dMovieErrorNone) 
	{
		vr3dMovieError videoErr = vr_3d_movie_decoder_open_video_stream();

	}
	if (err != vr3dMovieErrorNone) {
		;
		return false;
	}

	return true;
}

vr3dMovieError vr3dmoviedecoder::vr_3d_movie_decoder_open_input(const char* path)
{
	//分配一个AVFormatContext
	AVFormatContext* fmt_ctx = nullptr;
	fmt_ctx = avformat_alloc_context();
	if (!fmt_ctx)
		return vr3dMovieErrorOpenFile;

	//打开媒体文件并读取头
	if (avformat_open_input(&fmt_ctx, path, nullptr, nullptr) < 0) {
		if (fmt_ctx)
			avformat_free_context(fmt_ctx);
		return vr3dMovieErrorOpenFile;
	}

	//读取媒体文件的数据包以获取流信息
	if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
		avformat_close_input(&fmt_ctx);
		return vr3dMovieErrorStreamInfoNotFound;
	}

	//打印媒体信息
	av_dump_format(fmt_ctx, 0, path, 0);
	_fmt_ctx = fmt_ctx;

	return vr3dMovieErrorNone;
}

vr3dMovieError vr3dmoviedecoder::vr_3d_movie_decoder_open_video_stream_by_idx(unsigned int idx)
{
	// get a pointer to the codec context for the video stream
	AVCodecContext* codec_ctx = _fmt_ctx->streams[idx]->codec;

	// find the decoder for the video stream
	AVCodec* codec = avcodec_find_decoder(codec_ctx->codec_id);
	if (!codec)
		return vr3dMovieErrorCodecNotFound;

	// open codec
	if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
		return vr3dMovieErrorOpenCodec;

	// 分配一个 AVFrame 并将其字段设置为默认值。
	_video_frame = av_frame_alloc();
	if (!_video_frame) {
		avcodec_close(codec_ctx);
		return vr3dMovieErrorAllocateFrame;
	}

	_video_stream_idx = idx;
	_video_codec_ctx = codec_ctx;

	_frameWidth  = codec_ctx->width;
	_frameHeight = codec_ctx->height;
	_sampleRate  = codec_ctx->sample_rate;

	return vr3dMovieErrorNone;
}

vr3dMovieError vr3dmoviedecoder::vr_3d_movie_decoder_open_video_stream()
{
	vr3dMovieError errCode = vr3dMovieErrorStreamNotFound;

	//查找video stream idx
	_video_streams_idx = vr_3d_movie_decoder_collect_streams_idx(_fmt_ctx, AVMEDIA_TYPE_VIDEO);
	for (auto n : _video_streams_idx)
	{
		const unsigned int idx = n;
		if (0 == (_fmt_ctx->streams[idx]->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
			errCode = vr_3d_movie_decoder_open_video_stream_by_idx(idx);
			if (errCode == vr3dMovieErrorNone)
				break;
		}
		else {
			;
		}
	}

	return errCode;
}

vector<unsigned int> vr3dmoviedecoder::vr_3d_movie_decoder_collect_streams_idx(AVFormatContext* fmt_ctx, AVMediaType codec_type)
{
	vector<unsigned int> v_streams;
	for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i) {
		if (codec_type == fmt_ctx->streams[i]->codec->codec_type)
			v_streams.push_back(i);
	}
	return v_streams;
}
