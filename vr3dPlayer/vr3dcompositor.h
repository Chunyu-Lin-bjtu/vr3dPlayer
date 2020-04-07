#ifndef _VR_3D_COMPOSITOR_H_
#define _VR_3D_COMPOSITOR_H_
#include "common.h"
#include "vr3dscene.h"

class vr3dcompositor
{
public:
	vr3dcompositor();
	~vr3dcompositor();

public:
	bool vr_3d_compositor_init_scene();

	bool vr_3d_compositor_update_texture(/*AVFrame* pictureYUV*/);

	GLFWwindow* vr_3d_compositor_get_window();

	bool vr_3d_compositor_draw();

private:
	int vr_3d_compositor_init_gl_context();
private:
	vr3dscene* scene;
	GLFWwindow* window;
	int screen_w;
	int screen_h;
	GLuint textureId1;
};

#endif // !_VR_3D_COMPOSITOR_H_

