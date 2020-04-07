#ifndef _VR_3D_MESH_H_
#define _VR_3D_MESH_H_
//����C++��׼ͷ�ļ�
#include <list>
using namespace std;

// ������Ŀͷ�ļ�
#include "common.h"
#include "vr3dshader.h"

struct vr3dattributebuffer {
	const GLchar* name;
	GLuint vbo;
	size_t element_size;
	int vector_length;
};

class vr3dmesh
{
public:
	vr3dmesh();
	~vr3dmesh();

public:
	void vr_3d_mesh_new_plane(float aspect);




	void vr_3d_mesh_init_buffers();

	void vr_3d_mesh_bind_shader(vr3dshader* shader);
	void vr_3d_mesh_bind();
	void vr_3d_mesh_draw();
	void vr_3d_mesh_draw_mode(GLenum draw_mode);


	void vr_3d_mesh_upload_plane(float aspect);


	void vr_3d_mesh_append_attribute_buffer(const char* name, size_t element_size, int vector_length, GLfloat* vertices);

public:
	list<vr3dattributebuffer*> attribute_buffers;
	GLuint vao;
	GLuint ebo;

	GLuint index_size;		//������С
	GLuint vertex_count;	//�������

	GLenum draw_mode;		//����ģʽ
};
#endif // !_VR_3D_MESH_H_

