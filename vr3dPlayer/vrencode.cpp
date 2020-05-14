#include "vrEncode.h"


vrEncode::vrEncode()
{
	//注册封装，解封装，格式
	av_register_all();
	//注册解码器
	avcodec_register_all();
}

vrEncode::~vrEncode()
{
}


vrEncodeH264::vrEncodeH264(int w, int h, int fps) :
	_width(w), _height(h), _fps(fps)
{
	codec = nullptr;
	c = nullptr;
	oc = nullptr;
	ctx = nullptr;
	yuv = nullptr;
	rgb = nullptr;

	vr_encode_h264_init();
}

vrEncodeH264::~vrEncodeH264()
{
	av_write_trailer(oc);		//写文件尾
	avio_close(oc->pb);			//关闭视频输出IO
	avformat_free_context(oc);	//清理封装输出上下文
	avcodec_close(c);			//关闭编码器
	avcodec_free_context(&c);	//清理编码器上下文
	sws_freeContext(ctx);		//清理视频重采样上下文

	if (rgb) {
		delete rgb;
		rgb = nullptr;
	}
	//完成后解除映射
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
}

bool vrEncodeH264::vr_encode_h264_from_file(const char* src, const char* dst)
{
	int ret = 0;
	bool ok = false;
	//输入空间
	rgb = new uint8_t[_width * _height * 3];
	_ifp = fopen(src, "rb");
	if (!_ifp) {
		std::cout << "fopen file failed!" << std::endl;
		return false;
	}

	ok = vr_encode_h264_create_output_ctx(dst);
	if (!ok) {
		return false;
	}

	return true;
}

void vrEncodeH264::vr_encode_h264_run_file()
{
	int p = 0;
	for (;;)
	{
		int len = fread(rgb, 1, _width * _height * 3, _ifp);
		if (len <= 0) {
			break;
		}

		uint8_t* indata[AV_NUM_DATA_POINTERS] = { 0 };
		indata[0] = rgb;
		int inlinesize[AV_NUM_DATA_POINTERS] = { 0 };
		inlinesize[0] = _width * 3;

		int h = sws_scale(ctx, indata, inlinesize, 0, _height, yuv->data, yuv->linesize);
		if (h <= 0) {
			break;
		}

		if (!vr_encode_h264_frame()) {
			continue;
		}
	}

	return;
}

bool vrEncodeH264::vr_encode_h264_from_framebuffer_direct(const char* dst)
{
	//输入空间
	rgb = new uint8_t[_width * _height * 3];

	return vr_encode_h264_create_output_ctx(dst);
}

void vrEncodeH264::vr_encode_h264_run_framebuffer_direct()
{
	glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	uint8_t* indata[AV_NUM_DATA_POINTERS] = { 0 };
	indata[0] = rgb + _width * (_height - 1) * 3;
	int inlinesize[AV_NUM_DATA_POINTERS] = { 0 };
	inlinesize[0] = -_width * 3;

	int h = sws_scale(ctx, indata, inlinesize, 0, _height, yuv->data, yuv->linesize);
	if (h <= 0) {
		return;
	}

	vr_encode_h264_frame();
	
	return ;
}

bool vrEncodeH264::vr_encode_h264_from_framebuffer_map(int n, const char* dst)
{
	//输入空间
	rgb = new uint8_t[_width * _height * 3];
	_pbo = new GLuint[n];
	_n = n;
	//（1）创建顶点缓存
	glGenBuffers(n, _pbo);
	for (int i = 0; i < n; ++i)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[i]);
		glBufferData(GL_PIXEL_PACK_BUFFER, _width * _height * 3, nullptr, GL_STREAM_READ);
	}
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	return vr_encode_h264_create_output_ctx(dst);
}

void vrEncodeH264::vr_encode_h264_run_framebuffer_map()
{
	static int pboIdx = 0;
	static int nextpboIdx = 0;
	glReadBuffer(GL_FRONT); // 设置读取的FBO
	pboIdx = (pboIdx + 1) % _n;
	nextpboIdx = (pboIdx + 1) % _n;
	// 开始FBO到PBO复制操作 pack操作
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[pboIdx]);
	// OpenGL执行异步的DMA传输 这个命令会立即放回 此时CPU可以执行其他任务
	glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	
	//（3）把pbo中的数据映射到内存中
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[nextpboIdx]);
	glBufferData(GL_PIXEL_PACK_BUFFER, _width * _height * 3, 0, GL_STREAM_DRAW);
	uint8_t* rgb2 = (uint8_t*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	if (rgb2) {
		uint8_t* indata[AV_NUM_DATA_POINTERS] = { 0 };
		std::memcpy(rgb, rgb2, _width * _height * 3);
		indata[0] = rgb + _width * (_height - 1) * 3;
		int inlinesize[AV_NUM_DATA_POINTERS] = { 0 };
		inlinesize[0] = -_width * 3;

		int h = sws_scale(ctx, indata, inlinesize, 0, _height, yuv->data, yuv->linesize);
		if (h <= 0) {
			return;
		}
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // 释放映射的用户内存空间
	}
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	vr_encode_h264_frame();

	return;
}

bool vrEncodeH264::vr_encode_h264_init()
{
	//1 创建编码器 
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!codec)
	{
		cout << " avcodec_find_encoder AV_CODEC_ID_H264 failed!" << endl;
		return false;
	}
	//编码器上下文
	c = avcodec_alloc_context3(codec);
	if (!c)
	{
		cout << " avcodec_alloc_context3  failed!" << endl;
		return false;
	}

	//设置视频编码相关参数
	//比特率
	c->bit_rate = 400000000;

	c->width = _width;
	c->height = _height;
	//把1秒钟分成fps个单位 
	c->time_base = { 1,_fps };
	c->framerate = { _fps,1 };

	//画面组大小，就是多少帧出现一个关键帧
	//GOP 介绍  见   https://blog.csdn.net/xiangjai/article/details/44238005
	c->gop_size = 50;

	c->max_b_frames = 0;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	c->codec_id = AV_CODEC_ID_H264;
	c->thread_count = 8;

	//全局的编码信息
	c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	//打开编码器
	int ret = avcodec_open2(c, codec, nullptr);
	if (ret < 0)
	{
		cout << " avcodec_open2  failed!" << endl;
		return false;
	}
}

bool vrEncodeH264::vr_encode_h264_create_output_ctx(const char* dst)
{
	int ret = 0;
	//2 create out context
	avformat_alloc_output_context2(&oc, 0, 0, dst);

	//3 add video stream
	AVStream* st = avformat_new_stream(oc, nullptr);
	st->id = 0;
	st->codecpar->codec_tag = 0;
	avcodec_parameters_from_context(st->codecpar, c);

	cout << "===============================================" << endl;
	av_dump_format(oc, 0, dst, 1);
	cout << "===============================================" << endl;

	//4 rgb to yuv
	//改变视频尺寸
	ctx = sws_getCachedContext(ctx,
		_width, _height, AV_PIX_FMT_RGB24,
		_width, _height, AV_PIX_FMT_YUV420P, SWS_BICUBIC,
		nullptr, nullptr, nullptr);

	//输出空间
	yuv = av_frame_alloc();
	yuv->format = AV_PIX_FMT_YUV420P;
	yuv->width = _width;
	yuv->height = _height;

	//分配空间
	ret = av_frame_get_buffer(yuv, 32);
	if (ret < 0) {
		cout << " av_frame_get_buffer  failed!" << endl;
		return false;
	}

	//5 write mp4 head
	ret = avio_open(&oc->pb, dst, AVIO_FLAG_WRITE);
	if (ret < 0) {
		cout << " avio_open  failed!" << endl;
		return false;
	}

	ret = avformat_write_header(oc, nullptr);
	if (ret < 0) {
		cout << " avformat_write_header  failed!" << endl;
		return false;
	}

	return true;
}

bool vrEncodeH264::vr_encode_h264_frame()
{
	int ret = 0;
	static int p = 0;
	//6 encode frame
	yuv->pts = p;
	p = p + 3600;
	//发送到编码器
	ret = avcodec_send_frame(c, yuv);
	if (ret != 0){
		return false;
	}
	AVPacket pkt;
	av_init_packet(&pkt);
	//接收编码结果
	ret = avcodec_receive_packet(c, &pkt);
	if (ret != 0) {
		return false;
	}
	//将编码后的帧写入文件
	av_interleaved_write_frame(oc, &pkt);
	cout << "<" << pkt.size << ">";

	return true;
}

