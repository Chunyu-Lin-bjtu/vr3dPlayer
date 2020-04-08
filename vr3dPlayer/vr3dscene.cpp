#include "vr3dscene.h"


vr3dscene::vr3dscene(vr3dcamera* cam, void (*_init_func) (vr3dscene*))
{
	this->wireframe_mode = false;
	this->gl_initialized = false;
	node_draw_func = &vr_3d_node_draw;

	this->camera = cam;
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
	this->camera->vr_3d_camera_update_view();
#ifdef HAVE_OPENHMD
	//vr_3d_scene_init_stereo_renderer();
#endif
}

void vr3dscene::vr_3d_scene_draw_nodes(glm::mat4* mvp)
{
	list<vr3dnode*>::iterator it;
	for (it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		vr3dnode* node = *it;
		node->shader->vr_3d_shader_bind();
		//node->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(*mvp));
		glm::mat4 identiy = glm::mat4();	//TODO...先更新一个单位矩阵，鼠标键盘事件还未加入
		node->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(identiy));
		this->node_draw_func(node);
	}
}

void vr3dscene::vr_3d_scene_draw()
{
	this->camera->vr_3d_camera_update_view();
#ifdef HAVE_OPENHMD
	if (this->camera)
		if (use_shader_proj)
			vr_3d_renderer_draw_stereo_shader_proj(this->renderer);
		else
			vr_3d_renderer_draw_stereo(this->renderer);
	else
		vr_3d_scene_draw_nodes(&this->camera->mvp);
#else
	vr_3d_scene_draw_nodes(&this->camera->mvp);
#endif
	vr_3d_scene_clear_state();
}

void vr3dscene::vr_3d_scene_clear_state()
{
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
