#include "vr3devent.h"

vr3devent::vr3devent(GLFWwindow* w)
{
	//glfwSetKeyCallback(w, vr_3d_event_key_callback);					//设置键盘回调函数
	glfwSetCursorPosCallback(w, vr_3d_event_cursor_pos_callback);		//设置光标回调函数
	glfwSetMouseButtonCallback(w, vr_3d_event_mouse_button_callback);	//设置鼠标回调函数
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

void vr3devent::vr_3d_event_set_func(vr3dcamera* inst, vr_3d_event_func f)
{
	pair<vr3dcamera*, vr_3d_event_func> p = make_pair(inst, f);
	e->funcs.push_back(p);
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


void vr_3d_event_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	e->event_type = VR_3D_EVENT_MOUSE_BUTTON_PRESS;
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
		}
		e->vr_3d_event_set_int("key", key);
		for (int i = 0; i < e->funcs.size(); i++)
		{
			vr3dcamera* pInst = e->funcs[i].first;
			vr_3d_event_func f = e->funcs[i].second;
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

void vr_3d_event_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	e->event_type = VR_3D_EVENT_MOUSE_MOVE;
	e->vr_3d_event_set_double("pointer_x", xpos);
	e->vr_3d_event_set_double("pointer_y", ypos);
	for (int i = 0; i < e->funcs.size(); i++)
	{
		vr3dcamera* pInst = e->funcs[i].first;
		vr_3d_event_func f = e->funcs[i].second;
		(pInst->*f)(e);
	}
}

void vr_3d_event_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// button: 0，左键   1，右键   2，中轴轮
	if (action == GLFW_PRESS)
	{
		e->event_type = VR_3D_EVENT_MOUSE_BUTTON_PRESS;
	}
	else if (action == GLFW_RELEASE)
	{
		e->event_type = VR_3D_EVENT_MOUSE_BUTTON_RELEASE;
	}
	e->vr_3d_event_set_int("button", button);
	for (int i = 0; i < e->funcs.size(); i++)
	{
		vr3dcamera* pInst = e->funcs[i].first;
		vr_3d_event_func f = e->funcs[i].second;
		(pInst->*f)(e);
	}
}

