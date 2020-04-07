#ifndef _VR_3D_MESH_H_
#define _VR_3D_MESH_H_
//包含C++标准头文件
#include <list>
using namespace std;

// 包含项目头文件
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

	GLuint index_size;		//索引大小
	GLuint vertex_count;	//顶点个数

	GLenum draw_mode;		//绘制模式
};
#endif // !_VR_3D_MESH_H_

