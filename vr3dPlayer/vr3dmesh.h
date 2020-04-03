#ifndef _VR_3D_MESH_H_
#define _VR_3D_MESH_H_
// 包含项目头文件
#include "common.h"

//包含C++标准头文件
#include <list>
using namespace std;

struct vr3dattributebuffer {
	const GLchar* name;
	GLuint location;
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

public:
	list<vr3dattributebuffer*> attribute_buffers;
	GLuint vao;
	GLuint vbo_indices;

	GLuint index_size;
	GLuint vertex_count;

	GLenum draw_mode;
};
#endif // !_VR_3D_MESH_H_

