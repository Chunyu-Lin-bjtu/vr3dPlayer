#ifndef _VR_3D_PBO_H_
#define _VR_3D_PBO_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include "vr3dmedia.h"

class vr3dpbo
{
public:
	vr3dpbo(GLuint w, GLuint h);
	~vr3dpbo();

public:
	// unpack ==============

	void vr_3d_pbo_init_unpack_buffer();

	bool vr_3d_pbo_open_image_source(GLuint w, GLuint h);

	// 1.gpu�У���pbo����pixels��fbo
	void vr_3d_pbo_copy_pixels_to_fbo(GLuint idx);

	// 2.ӳ��pbo���û��ռ䣬������pixels��������cpu��gpu�Ŀ���
	void vr_3d_pbo_map_memery_and_update_pixels(GLuint idx);

public:
	GLuint vr_3d_pbo_init_pixels_texture();

private:
	//ͼƬ�Ŀ���
	GLuint width;
	GLuint height;

	vr3dmedia* vdo;

	GLuint pboids[2];
	GLuint data_size;
	GLuint pixelTextId;
	GLenum pixel_format;
	GLubyte* buf_pbo_2_memery;// ��������

};

#endif // !_VR_3D_PBO_H_

