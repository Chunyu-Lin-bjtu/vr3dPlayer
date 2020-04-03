#include "vr3dmesh.h"

vr3dmesh::vr3dmesh()
{
}

vr3dmesh::~vr3dmesh()
{
}

void vr3dmesh::vr_3d_mesh_new_plane(float aspect)
{
	vr_3d_mesh_init_buffers();
}

void vr3dmesh::vr_3d_mesh_init_buffers()
{
	// 创建并绑定VAO对象
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	// 创建VBO对象
	glGenBuffers(1, &this->vbo_indices);
}
