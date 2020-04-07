#include "vr3dnode.h"

void vr_3d_node_draw(vr3dnode* node)
{
	list<vr3dmesh*>::iterator it;
	for (it = node->meshes.begin(); it != node->meshes.end(); ++it)
	{
		vr3dmesh* mesh = *it;
		mesh->vr_3d_mesh_bind();
		mesh->vr_3d_mesh_draw();
	}
}

void vr_3d_node_draw_wireframe(vr3dnode* node)
{
	list<vr3dmesh*>::iterator it;
	for (it = node->meshes.begin(); it != node->meshes.end(); ++it)
	{
		vr3dmesh* mesh = *it;
		mesh->vr_3d_mesh_bind();
		mesh->vr_3d_mesh_draw_mode(GL_LINE_STRIP);
	}
}

vr3dnode::vr3dnode()
{
}

vr3dnode::~vr3dnode()
{
}

// 绘制坐标系
void vr3dnode::vr_3d_node_new_debug_axes()
{
	this->shader = new vr3dshader("shader/mvp_color.vert", "shader/color.frag");

	if (this->shader == NULL) {
		printf("Failed to create shaders.\n");
		return;
	}
	this->shader->vr_3d_shader_bind();

	glm::vec3 from(0.f, 0.f, 0.f);
	glm::vec3 to(1.f, 0.f, 0.f);
	glm::vec3 color(1.f, 0.f, 0.f);

	vr3dmesh* x_axis = new vr3dmesh();
	x_axis->vr_3d_mesh_new_line(&from, &to, &color);
	x_axis->vr_3d_mesh_bind_shader(this->shader);
	this->meshes.push_back(x_axis);

	from = glm::vec3(0.f, 0.f, 0.f);
	to = glm::vec3(0.f, 1.f, 0.f);
	color = glm::vec3(0.f, 1.f, 0.f);

	vr3dmesh* y_axis = new vr3dmesh();
	y_axis->vr_3d_mesh_new_line(&from, &to, &color);
	y_axis->vr_3d_mesh_bind_shader(this->shader);
	this->meshes.push_back(y_axis);

	from = glm::vec3(0.f, 0.f, 0.f);
	to = glm::vec3(0.f, 0.f, 1.f);
	color = glm::vec3(0.f, 0.f, 1.f);

	vr3dmesh* z_axis = new vr3dmesh();
	z_axis->vr_3d_mesh_new_line(&from, &to, &color);
	z_axis->vr_3d_mesh_bind_shader(this->shader);
	this->meshes.push_back(z_axis);
}

vr3dnode* vr3dnode::vr_3d_node_new_from_mesh_shader(vr3dmesh* mesh, vr3dshader* shader)
{
	vr3dnode* node = new vr3dnode();
	node->meshes.push_back(mesh);
	node->shader = shader;

	node->shader->vr_3d_shader_bind();
	mesh->vr_3d_mesh_bind_shader(shader);

	return node;
}

