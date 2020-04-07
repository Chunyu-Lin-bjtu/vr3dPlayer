#ifndef _VR_3D_NODE_H_
#define _VR_3D_NODE_H_
#include <list>
#include "vr3dmesh.h"
using namespace std;

class vr3dnode;
void vr_3d_node_draw(vr3dnode* node);
void vr_3d_node_draw_wireframe(vr3dnode* node);

class vr3dnode
{
public:
	vr3dnode();
	~vr3dnode();

public:
	void vr_3d_node_new_debug_axes();

	static vr3dnode* vr_3d_node_new_from_mesh_shader(vr3dmesh* mesh, vr3dshader* shader);
public:
	list<vr3dmesh*> meshes;
	vr3dshader* shader;
};

#endif // !_VR_3D_NODE_H_
