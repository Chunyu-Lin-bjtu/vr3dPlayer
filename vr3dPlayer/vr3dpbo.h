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

	// 1.gpu中，从pbo拷贝pixels到fbo
	void vr_3d_pbo_copy_pixels_to_fbo(GLuint idx);

	// 2.映射pbo到用户空间，并更新pixels，减少了cpu到gpu的拷贝
	void vr_3d_pbo_map_memery_and_update_pixels(GLuint idx);

public:
	GLuint vr_3d_pbo_init_pixels_texture();

private:
	//图片的宽，高
	GLuint width;
	GLuint height;

	vr3dmedia* vdo;

	GLuint pboids[2];
	GLuint data_size;
	GLuint pixelTextId;
	GLenum pixel_format;
	GLubyte* buf_pbo_2_memery;// 像素数据

};

#endif // !_VR_3D_PBO_H_

