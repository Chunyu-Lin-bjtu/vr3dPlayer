#include "vr3devent.h"

vr3devent::vr3devent(GLFWwindow* w)
{
	glfwSetKeyCallback(w, vr_3d_event_key_callback);					//���ü��̻ص�����
	glfwSetCursorPosCallback(w, vr_3d_event_cursor_pos_callback);		//���ù��ص�����
	glfwSetMouseButtonCallback(w, vr_3d_event_mouse_button_callback);	//�������ص�����
}

vr3devent::~vr3devent()
{
}

static vr3devent* e = nullptr;
vr3devent* vr3devent::vr_3d_event_create(GLFWwindow* w)
{
	if (e)
	{
		return e;
	}
	
	e = new vr3devent(w);

	return e;
}

void vr3devent::vr_3d_event_set_camera_func(vr3dcamera* inst, vr_3d_event_camera_func f)
{
	pair<vr3dcamera*, vr_3d_event_camera_func> p = make_pair(inst, f);
	e->camera_funcs.push_back(p);
}

void vr3devent::vr_3d_event_set_scene_func(vr3dscene* inst, vr_3d_event_scene_func f)
{
	pair<vr3dscene*, vr_3d_event_scene_func> p = make_pair(inst, f);
	e->scene_funcs.push_back(p);
}

eventType vr3devent::vr_3d_event_get_type()
{
	return event_type;
}

double vr3devent::vr_3d_event_get_double(const char* name)
{
	return edouble[name];
}

int vr3devent::vr_3d_event_get_int(const char* name)
{
	return eint[name];
}


void vr3devent::vr_3d_event_set_double(const char* name, double value)
{
	edouble[name] = value;
}

void vr3devent::vr_3d_event_set_int(const char* name, int value)
{
	eint[name] = value;
}

// glfw���̵Ļص�����
void vr_3d_event_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	e->event_type = VR_3D_EVENT_KEY_PRESS;
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE); // �رմ���
		}
		e->vr_3d_event_set_int("key", key);
		//����camera�Ļص�
		for (int i = 0; i < e->camera_funcs.size(); i++)
		{
			vr3dcamera* pInst = e->camera_funcs[i].first;
			vr_3d_event_camera_func f = e->camera_funcs[i].second;
			(pInst->*f)(e);
		}
		//����scene�Ļص�
		for (int i = 0; i < e->scene_funcs.size(); i++)
		{
			vr3dscene* pInst = e->scene_funcs[i].first;
			vr_3d_event_scene_func f = e->scene_funcs[i].second;
			(pInst->*f)(e);
		}
	}
	else if (action == GLFW_RELEASE)
	{

	}
	else
	{
	}
}

// glfw���Ļص�����
void vr_3d_event_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	e->event_type = VR_3D_EVENT_MOUSE_MOVE;
	e->vr_3d_event_set_double("pointer_x", xpos);
	e->vr_3d_event_set_double("pointer_y", ypos);
	for (int i = 0; i < e->camera_funcs.size(); i++)
	{
		vr3dcamera* pInst = e->camera_funcs[i].first;
		vr_3d_event_camera_func f = e->camera_funcs[i].second;
		(pInst->*f)(e);
	}
}

void vr_3d_event_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// button: 0�����   1���Ҽ�   2��������
	if (action == GLFW_PRESS)
	{
		e->event_type = VR_3D_EVENT_MOUSE_BUTTON_PRESS;
	}
	else if (action == GLFW_RELEASE)
	{
		e->event_type = VR_3D_EVENT_MOUSE_BUTTON_RELEASE;
	}
	e->vr_3d_event_set_int("button", button);
	for (int i = 0; i < e->camera_funcs.size(); i++)
	{
		vr3dcamera* pInst = e->camera_funcs[i].first;
		vr_3d_event_camera_func f = e->camera_funcs[i].second;
		(pInst->*f)(e);
	}
}

