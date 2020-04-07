#include "vr3dscene.h"


vr3dscene::vr3dscene(vr3dcamera* cam, void (*_init_func) (vr3dscene*))
{
	this->gl_initialized = false;
	node_draw_func = &vr_3d_node_draw;

	gl_init_func = _init_func;
}

vr3dscene::~vr3dscene()
{
}

void vr3dscene::vr_3d_scene_append_node(vr3dnode* node)
{
	this->nodes.push_back(node);
}

void vr3dscene::vr_3d_scene_init_gl()
{
	if (this->gl_initialized)
		return;

	this->gl_init_func(this);
	this->gl_initialized = true;
	//this->camera->vr_3d_camera_update_view();

	//vr_3d_scene_init_stereo_renderer();
}

void vr3dscene::vr_3d_scene_draw_nodes(glm::mat4* mvp)
{
	list<vr3dnode*>::iterator it;
	for (it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		vr3dnode* node = *it;
		node->shader->vr_3d_shader_bind();
		//node->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(*mvp));
		this->node_draw_func(node);
	}
}

void vr3dscene::vr_3d_scene_draw()
{
	vr_3d_scene_draw_nodes(nullptr);
}
