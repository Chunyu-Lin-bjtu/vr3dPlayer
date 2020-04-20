#include "vr3dscene.h"
#include "vr3devent.h"
#ifdef HAVE_OPENHMD
#include "vr3dcamera_hmd.h"
#endif

bool use_shader_proj = false;
//bool use_shader_proj = TRUE;

vr3dscene::vr3dscene(vr3dcamera* cam, void (*_init_func) (vr3dscene*))
{
	wireframe_mode = false;
	camera = nullptr;
	renderer = nullptr;
	gl_initialized = false;
	node_draw_func = &vr_3d_node_draw;

	this->camera = cam;
	gl_init_func = _init_func;

	// 设置scene 事件
	vr3devent::vr_3d_event_set_scene_func(this, &vr3dscene::vr_3d_scene_navigation_event);
}

vr3dscene::~vr3dscene()
{
	nodes.clear();
}

void vr3dscene::vr_3d_scene_init_gl()
{
	if (this->gl_initialized)
		return;

	gl_init_func(this);
	gl_initialized = true;
	camera->vr_3d_camera_update_view();
#ifdef HAVE_OPENHMD
	vr_3d_scene_init_stereo_renderer();
#endif
}

void vr3dscene::vr_3d_scene_draw_nodes(glm::mat4* mvp)
{
	list<vr3dnode*>::iterator it;
	for (it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		vr3dnode* node = *it;
		node->shader->vr_3d_shader_bind();
#if 1
		node->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(*mvp));
#else
		glm::mat4 identiy = glm::mat4();	//TODO...先更新一个单位矩阵，鼠标键盘事件还未加入
		node->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(identiy));
#endif // 0

		this->node_draw_func(node);
	}
}

void vr3dscene::vr_3d_scene_draw()
{
	camera->vr_3d_camera_update_view();
#ifdef HAVE_OPENHMD
	if (camera)
		if (use_shader_proj)
			renderer->vr_3d_renderer_draw_stereo_shader_proj(this);
		else
			renderer->vr_3d_renderer_draw_stereo(this);
			
	else
		vr_3d_scene_draw_nodes(&camera->mvp);
#else
	vr_3d_scene_draw_nodes(&camera->mvp);
#endif
	vr_3d_scene_clear_state();
}

// 把节点追加到scene，场景中
void vr3dscene::vr_3d_scene_append_node(vr3dnode* node)
{
	this->nodes.push_back(node);
}

void vr3dscene::vr_3d_scene_toggle_wireframe_mode()
{
	if (wireframe_mode) {
		wireframe_mode = false;
		node_draw_func = &vr_3d_node_draw;
	}
	else {
		wireframe_mode = true;
		node_draw_func = &vr_3d_node_draw_wireframe;
	}
}

void vr3dscene::vr_3d_scene_navigation_event(vr3devent* e)
{
	eventType event_type = e->vr_3d_event_get_type();

	switch (event_type)
	{
	case VR_3D_EVENT_KEY_PRESS: {
		int key = e->vr_3d_event_get_int("key");
		if (key == GLFW_KEY_TAB) {
			vr_3d_scene_toggle_wireframe_mode();
		}
		break;
	}
	default:
		break;
	}
}

// 清除opengl渲染状态，解绑
void vr3dscene::vr_3d_scene_clear_state()
{
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

/* stereo */

#ifdef HAVE_OPENHMD
bool vr3dscene::vr_3d_scene_init_hmd()
{
	vr3dhmd* hmd = ((vr3dcamera_hmd*)camera)->hmd;
	if (!hmd->device) return false;
	
	return true;
}

void vr3dscene::vr_3d_scene_init_stereo_renderer()
{
	renderer = new vr3drenderer();
	if (camera) {
		vr3dcamera_hmd* hmd_cam = (vr3dcamera_hmd*)(camera);
		vr3dhmd* hmd = hmd_cam->hmd;
		renderer->vr_3d_renderer_stereo_init_from_hmd(hmd);

		if (use_shader_proj)
			renderer->vr_3d_renderer_init_stereo_shader_proj(camera);
		else
			renderer->vr_3d_renderer_init_stereo(camera);
	}
}
#endif
