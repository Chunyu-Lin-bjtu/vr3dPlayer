#ifndef _VR_3D_EVENT_H_
#define _VR_3D_EVENT_H_
#include <map>
#include <vector>

#include "common.h"
#include "vr3dscene.h"
#include "vr3dcamera.h"

using namespace std;

class vr3devent;

typedef void (vr3dcamera ::* vr_3d_event_camera_func)(vr3devent*);
typedef void (vr3dscene ::* vr_3d_event_scene_func)(vr3devent*);
void vr_3d_event_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void vr_3d_event_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void vr_3d_event_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

enum eventType {
	VR_3D_EVENT_MOUSE_MOVE,
	VR_3D_EVENT_MOUSE_BUTTON_RELEASE,
	VR_3D_EVENT_MOUSE_BUTTON_PRESS,
	VR_3D_EVENT_KEY_PRESS,
};

class vr3devent
{
public:
	vr3devent(GLFWwindow* w);
	~vr3devent();
public:

public:
	static vr3devent* vr_3d_event_create(GLFWwindow* w);
	static void vr_3d_event_set_camera_func(vr3dcamera* inst, vr_3d_event_camera_func f);
	static void vr_3d_event_set_scene_func(vr3dscene* inst, vr_3d_event_scene_func f);
public:
	//获取事件类型
	eventType vr_3d_event_get_type();

	double vr_3d_event_get_double(const char* name);
	int vr_3d_event_get_int(const char* name);
	
	void vr_3d_event_set_double(const char* name, double value);
	void vr_3d_event_set_int(const char* name, int value);
public:
	eventType event_type;
	map<const char*, double> edouble;
	map<const char*, int> eint;
	vector< pair<vr3dcamera*, vr_3d_event_camera_func> > camera_funcs;
	vector< pair<vr3dscene*, vr_3d_event_scene_func> > scene_funcs;
};

#endif // !_VR_3D_EVENT_H_
