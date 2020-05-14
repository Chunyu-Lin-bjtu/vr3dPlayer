#include "vrEncode.h"


vrEncode::vrEncode()
{
	//ע���װ�����װ����ʽ
	av_register_all();
	//ע�������
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
	av_write_trailer(oc);		//д�ļ�β
	avio_close(oc->pb);			//�ر���Ƶ���IO
	avformat_free_context(oc);	//�����װ���������
	avcodec_close(c);			//�رձ�����
	avcodec_free_context(&c);	//���������������
	sws_freeContext(ctx);		//������Ƶ�ز���������

	if (rgb) {
		delete rgb;
		rgb = nullptr;
	}
	//��ɺ���ӳ��
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
}

bool vrEncodeH264::vr_encode_h264_from_file(const char* src, const char* dst)
{
	int ret = 0;
	bool ok = false;
	//����ռ�
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
	//����ռ�
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
	//����ռ�
	rgb = new uint8_t[_width * _height * 3];
	_pbo = new GLuint[n];
	_n = n;
	//��1���������㻺��
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
	glReadBuffer(GL_FRONT); // ���ö�ȡ��FBO
	pboIdx = (pboIdx + 1) % _n;
	nextpboIdx = (pboIdx + 1) % _n;
	// ��ʼFBO��PBO���Ʋ��� pack����
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[pboIdx]);
	// OpenGLִ���첽��DMA���� �������������Ż� ��ʱCPU����ִ����������
	glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	
	//��3����pbo�е�����ӳ�䵽�ڴ���
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
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER); // �ͷ�ӳ����û��ڴ�ռ�
	}
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	vr_encode_h264_frame();

	return;
}

bool vrEncodeH264::vr_encode_h264_init()
{
	//1 ���������� 
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!codec)
	{
		cout << " avcodec_find_encoder AV_CODEC_ID_H264 failed!" << endl;
		return false;
	}
	//������������
	c = avcodec_alloc_context3(codec);
	if (!c)
	{
		cout << " avcodec_alloc_context3  failed!" << endl;
		return false;
	}

	//������Ƶ������ز���
	//������
	c->bit_rate = 400000000;

	c->width = _width;
	c->height = _height;
	//��1���ӷֳ�fps����λ 
	c->time_base = { 1,_fps };
	c->framerate = { _fps,1 };

	//�������С�����Ƕ���֡����һ���ؼ�֡
	//GOP ����  ��   https://blog.csdn.net/xiangjai/article/details/44238005
	c->gop_size = 50;

	c->max_b_frames = 0;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	c->codec_id = AV_CODEC_ID_H264;
	c->thread_count = 8;

	//ȫ�ֵı�����Ϣ
	c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	//�򿪱�����
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
	//�ı���Ƶ�ߴ�
	ctx = sws_getCachedContext(ctx,
		_width, _height, AV_PIX_FMT_RGB24,
		_width, _height, AV_PIX_FMT_YUV420P, SWS_BICUBIC,
		nullptr, nullptr, nullptr);

	//����ռ�
	yuv = av_frame_alloc();
	yuv->format = AV_PIX_FMT_YUV420P;
	yuv->width = _width;
	yuv->height = _height;

	//����ռ�
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
	//���͵�������
	ret = avcodec_send_frame(c, yuv);
	if (ret != 0){
		return false;
	}
	AVPacket pkt;
	av_init_packet(&pkt);
	//���ձ�����
	ret = avcodec_receive_packet(c, &pkt);
	if (ret != 0) {
		return false;
	}
	//��������֡д���ļ�
	av_interleaved_write_frame(oc, &pkt);
	cout << "<" << pkt.size << ">";

	return true;
}

