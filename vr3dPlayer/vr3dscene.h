﻿#ifndef _VR_3D_SCENE_H_
#define _VR_3D_SCENE_H_
#include <list>

#include "vr3dnode.h"
#include "vr3dcamera.h"

// 引入GLM库
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

using namespace std;

class vr3dscene;
typedef void(*vr_3d_node_draw_func) (vr3dnode*);
typedef void(*vr_3d_gl_init_func) (vr3dscene*);

class vr3dscene
{
public:
	vr3dscene(vr3dcamera* cam, void (*_init_func) (vr3dscene*));
	~vr3dscene();

public:
	void vr_3d_scene_append_node(vr3dnode* node);
	//void vr_3d_scene_toggle_wireframe_mode();
	void vr_3d_scene_init_gl();

	void vr_3d_scene_draw_nodes(glm::mat4* mvp);
	void vr_3d_scene_draw();

	void vr_3d_scene_clear_state();
public:
	
	bool gl_initialized;
	bool wireframe_mode;

	vr_3d_node_draw_func node_draw_func;
	vr_3d_gl_init_func   gl_init_func;

	vr3dcamera* camera;
	list<vr3dnode*> nodes;
};

#endif // !_VR_3D_SCENE_H_

