#include <string>
#include "vr3dpbo.h"


vr3dpbo::vr3dpbo(GLuint w, GLuint h) :
	data_size(w*h*3),width(w),height(h)
{
	buf_pbo_2_memery = new GLubyte[data_size];
	memset(buf_pbo_2_memery, 0, data_size);
	pixel_format = GL_RGB;

	vdo = new vr3dmedia_opencv();
}

vr3dpbo::~vr3dpbo()
{
}

void vr3dpbo::vr_3d_pbo_init_unpack_buffer()
{
	glGenBuffers(2, pboids);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboids[0]);
	//  GL_STREAM_DRAW���ڴ������ݵ�texture object GL_STREAM_READ ���ڶ�ȡFBO������
	glBufferData(GL_PIXEL_UNPACK_BUFFER, data_size, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboids[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, data_size, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	return;
}

bool vr3dpbo::vr_3d_pbo_open_image_source(GLuint w, GLuint h)
{
	bool ret = false;
	const char* name = "D:\\CPlusPlus\\��ý�幤��\\ffmpeg\\3GlassFFmpeg\\video\\bbb_sunflower_1080p_60fps_normal.mp4";
	ret = vdo->vr_3d_media_open_file(name, w, h);
	if (!ret) {
		std::cerr << "vr_3d_compositor_init_scene::opencv can not open video.  " << name << std::endl;
		char waitKey;
		std::cin >> waitKey;
		return false;
	}
	return true;
}

void vr3dpbo::vr_3d_pbo_copy_pixels_to_fbo(GLuint idx)
{
	// �������PBO
	glBindTexture(GL_TEXTURE_2D, pixelTextId);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboids[idx]);

	// ��PBO���Ƶ�texture object ʹ��ƫ���� ������ָ��
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, pixel_format, GL_UNSIGNED_BYTE, 0);
}

void vr3dpbo::vr_3d_pbo_map_memery_and_update_pixels(GLuint idx)
{
	bool ret = false;
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboids[idx]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, data_size, 0, GL_STREAM_DRAW);
	// ��PBOӳ�䵽�û��ڴ�ռ� Ȼ���޸�PBO������
	GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	if (ptr)
	{
		// ����ӳ�����ڴ�����
		ret = vdo->vr_3d_media_get_next_frame(ptr);
		if (!ret)
		{
			std::cerr << "vr_3d_video_get_next_frame can not get next frame." << std::endl;
		}
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); // �ͷ�ӳ����û��ڴ�ռ�
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GLuint vr3dpbo::vr_3d_pbo_init_pixels_texture()
{
	glGenTextures(1, &pixelTextId);
	glBindTexture(GL_TEXTURE_2D, pixelTextId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, (GLvoid*)buf_pbo_2_memery);
	glBindTexture(GL_TEXTURE_2D, 0);

	return pixelTextId;
}
