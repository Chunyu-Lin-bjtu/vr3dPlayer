#include "vr3dmedia.h"

vr3dmedia::vr3dmedia()
{
}

vr3dmedia::~vr3dmedia()
{
}


vr3dmedia_ffmpeg::vr3dmedia_ffmpeg()
{
	fmt_ctx = nullptr;
	video_stream = nullptr;
	video_codec_ctx = nullptr;
	video_decoder = nullptr;
	av_frame = nullptr;
	gl_frame = nullptr;
	packet = nullptr;
	sws_ctx = nullptr;

	/* 1. ע�����еķ�װ���ͽ��װ�� */
	av_register_all();
	avformat_network_init();
}

vr3dmedia_ffmpeg::~vr3dmedia_ffmpeg()
{
	avformat_close_input(&fmt_ctx);

	if (av_frame) av_free(av_frame);
	if (gl_frame) av_free(gl_frame);
	if (packet) av_free(packet);
	if (video_codec_ctx) avcodec_close(video_codec_ctx);
	if (fmt_ctx) avformat_free_context(fmt_ctx);
}

bool vr3dmedia_ffmpeg::vr_3d_media_open_file(const char* path, int w, int h, int* perror)
{
	vrMediaError errCode = vrMediaErrorNone;

	//_path = (char*)path;
	errCode = vr_media_ffmpeg_open_input(path);
	if (errCode == vrMediaErrorNone)
	{
		/* 4. Ѱ�Ҳ��򿪽����� */
		vrMediaError videoErr = vr_media_ffmpeg_open_video_stream();
		vrMediaError audioErr = vr_media_ffmpeg_open_audio_stream();
		if (videoErr != vrMediaErrorNone &&
			audioErr != vrMediaErrorNone) {

			errCode = videoErr; // both fails

		}
	}

	if (errCode != vrMediaErrorNone) {
		return false;
	}

	resize_width = w;
	resize_height = h;
	frame_size = w * h * 3;

	vr_3d_media_init_rgb24();

	return true;
}

bool vr3dmedia_ffmpeg::vr_3d_media_open_screen(int w, int h, int* perror)
{
	//����һ��AVFormatContext
	fmt_ctx = avformat_alloc_context();
	if (!fmt_ctx)
		return false;

	//Register Device  
	avdevice_register_all();

	/* 2. ���� */
	//Use gdigrab  
	AVDictionary* options = NULL;
	//Set some options  
	//grabbing frame rate  
	av_dict_set(&options, "framerate", "60", 0);
	//The distance from the left edge of the screen or desktop  
	av_dict_set(&options, "offset_x", "20", 0);
	//The distance from the top edge of the screen or desktop  
	av_dict_set(&options, "offset_y", "40", 0);
	//Video frame size. The default is to capture the full screen  
	av_dict_set(&options, "video_size", "640x480", 0);
	AVInputFormat* ifmt = av_find_input_format("gdigrab");
	if (avformat_open_input(&fmt_ctx, "desktop", ifmt, &options) != 0) {
		printf("Couldn't open input stream.\n");
		return false;
	}

	/* 3. ��ȡ������Ϣ */
	if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
		std::cout << "failed to get stream info" << std::endl;
		avformat_close_input(&fmt_ctx);
		return false;
	}

	/* 4. Ѱ�Ҳ��򿪽����� */
	vrMediaError videoErr = vr_media_ffmpeg_open_video_stream();

	resize_width = w;
	resize_height = h;
	frame_size = w * h * 3;

	vr_3d_media_init_rgb24();

	return true;
}

void vr3dmedia_ffmpeg::vr_3d_media_init_rgb24()
{
	// allocate the video frames
	av_frame = av_frame_alloc();
	gl_frame = av_frame_alloc();

	// ffmpeg����������ʽ��ͼƬ����Ҫ�����ֽ����洢 AV_PIX_FMT_RGB24: width * height * 3
	int size = avpicture_get_size(AV_PIX_FMT_RGB24, resize_width, resize_height);

	// ����ռ������ͼƬ���ݣ�����Դ���ݺ�Ŀ������
	uint8_t* internal_buffer = (uint8_t*)av_malloc(size * sizeof(uint8_t));

	//ǰ���av_frame_alloc������ֻ��Ϊ���AVFrame�ṹ��������ڴ棬�������͵�dataָ��ָ����ڴ滹û���䡣  
	//�����av_malloc�õ����ڴ��AVFrame������������Ȼ���仹������AVFrame��������Ա
	avpicture_fill((AVPicture*)gl_frame, internal_buffer, AV_PIX_FMT_RGB24, resize_width, resize_height);
	packet = (AVPacket*)av_malloc(sizeof(AVPacket));
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
	int ret = 0;
	do
	{
		/* 5. ��������, δ��������ݴ����packet */
		if (av_read_frame(fmt_ctx, packet) < 0)
		{
			av_free_packet(packet);
			return false;
		}
		/* 6. ����, ���������ݴ���� video_frame */
		/* ��Ƶ���� */
		if (packet->stream_index == video_streams_idx)
		{
			if (avcodec_decode_video2(video_codec_ctx, av_frame, &ret, packet) < 0)
			{
				av_free_packet(packet);
				return false;
			}

			if (0 == ret)
			{
				std::cout << "video decodec error!" << std::endl;
				continue;
			}

			if (!sws_ctx)
			{
				sws_ctx = sws_getContext(video_codec_ctx->width,
					video_codec_ctx->height, video_codec_ctx->pix_fmt,
					resize_width, resize_height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
			}

			sws_scale(sws_ctx, av_frame->data, av_frame->linesize, 0,
				video_codec_ctx->height, gl_frame->data, gl_frame->linesize);

			std::memcpy(buf, gl_frame->data[0], frame_size);
			/*glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, video_codec_ctx->width,
				video_codec_ctx->height, GL_RGB, GL_UNSIGNED_BYTE,
				gl_frame->data[0]);*/
		}

		/* ��Ƶ���� */
		if (packet->stream_index == audio_streams_idx)
		{
			if (avcodec_decode_audio4(audio_codec_ctx, av_frame, &ret, packet) < 0)
			{
				av_free_packet(packet);
				return false;
			}
		}

		av_free_packet(packet);
	} while (packet->stream_index != video_streams_idx);

	return true;
}

vrMediaError vr3dmedia_ffmpeg::vr_media_ffmpeg_open_input(const char* path)
{
	//����һ��AVFormatContext
	fmt_ctx = avformat_alloc_context();
	if (!fmt_ctx)
		return vrMediaErrorOpenFile;

	/* 2. ���� */
	if (avformat_open_input(&fmt_ctx, path, nullptr, nullptr) < 0) {
		std::cout << "failed to open input" << std::endl;
		if (fmt_ctx)
			avformat_free_context(fmt_ctx);
		return vrMediaErrorOpenFile;
	}

	/* 3. ��ȡ������Ϣ */
	if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
		std::cout << "failed to get stream info" << std::endl;
		avformat_close_input(&fmt_ctx);
		return vrMediaErrorStreamInfoNotFound;
	}

	//��ӡý����Ϣ
	av_dump_format(fmt_ctx, 0, path, 0);

	return vrMediaErrorNone;
}

vrMediaError vr3dmedia_ffmpeg::vr_media_ffmpeg_open_video_stream()
{
	vrMediaError errCode = vrMediaErrorStreamNotFound;

	//����video stream idx
	video_streams_idx = vr_media_ffmpeg_collect_streams_idx(fmt_ctx, AVMEDIA_TYPE_VIDEO);
	if (video_streams_idx == -1)
	{
		std::cout << "failed to find video stream" << std::endl;
		return errCode;
	}

	if (0 == (fmt_ctx->streams[video_streams_idx]->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
		errCode = vr_media_ffmpeg_open_video_stream_by_idx();
	}

	return errCode;
}

vrMediaError vr3dmedia_ffmpeg::vr_media_ffmpeg_open_audio_stream()
{
	vrMediaError errCode = vrMediaErrorStreamNotFound;
	audio_streams_idx = vr_media_ffmpeg_collect_streams_idx(fmt_ctx, AVMEDIA_TYPE_AUDIO);
	if (audio_streams_idx == -1)
	{
		std::cout << "failed to find audio stream" << std::endl;
		return vrMediaErrorStreamNotFound;
	}
	errCode = vr_media_ffmpeg_open_audio_stream_by_idx();

	return errCode;
}

vrMediaError vr3dmedia_ffmpeg::vr_media_ffmpeg_open_video_stream_by_idx()
{
	// get a pointer to the codec context for the video stream
	video_stream = fmt_ctx->streams[video_streams_idx];
	video_codec_ctx = video_stream->codec;

	// find the decoder for the video stream
	video_decoder = avcodec_find_decoder(video_codec_ctx->codec_id);
	if (!video_decoder)
	{
		std::cout << "failed to find video decoder" << std::endl;
		return vrMediaErrorCodecNotFound;
	}

	// open the decoder
	if (avcodec_open2(video_codec_ctx, video_decoder, nullptr) < 0)
	{
		std::cout << "failed to open video decoder" << std::endl;
		return vrMediaErrorOpenCodec;
	}

	return vrMediaErrorNone;
}

vrMediaError vr3dmedia_ffmpeg::vr_media_ffmpeg_open_audio_stream_by_idx()
{
	audio_stream = fmt_ctx->streams[audio_streams_idx];
	audio_codec_ctx = audio_stream->codec;

	// find the decoder for the audio stream
	audio_decoder = avcodec_find_decoder(audio_codec_ctx->codec_id);
	if (!audio_decoder)
	{
		std::cout << "failed to find audio decoder" << std::endl;
		return vrMediaErrorCodecNotFound;
	}

	// open the decoder
	if (avcodec_open2(audio_codec_ctx, audio_decoder, nullptr) < 0)
	{
		std::cout << "failed to open audio decoder" << std::endl;
		return vrMediaErrorOpenCodec;
	}

	return vrMediaErrorNone;
}

unsigned int vr3dmedia_ffmpeg::vr_media_ffmpeg_collect_streams_idx(AVFormatContext* fmt_ctx, AVMediaType codec_type)
{
	// ��ȡ���±�
	int idx = -1;
	for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i)
	{
		if (fmt_ctx->streams[i]->codec->codec_type == codec_type)
		{
			idx = i;
			break;
		}
	}

	return idx;
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
	//_cap.open(0); // ��ϵͳcamera
	if (!_cap.isOpened()) {
		std::cout << "vr3dmedia_opencv:: can not open video. " << path << std::endl;
		return false;
	}

	resize_width = w;
	resize_height = h;

	frame_size = w * h * 3;

	return true;
}

bool vr3dmedia_opencv::vr_3d_media_open_screen(int w, int h, int* perror)
{
	return false;
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