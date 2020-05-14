#ifndef _VR_3D_RENDERER_H_
#define _VR_3D_RENDERER_H_

#include "vr3dmesh.h"
#include "vr3dshader.h"
#include "vr3dcamera.h"

#ifdef HAVE_OPENHMD
#include "vr3dhmd.h"
#endif

class vr3dscene;
class vrEncodeH264;
class vr3drenderer
{
public:
	vr3drenderer();
	~vr3drenderer();

public:

	void vr_3d_renderer_init_stereo(vr3dcamera* cam);
	void vr_3d_renderer_draw_stereo(vr3dscene* scene);

	void vr_3d_renderer_init_stereo_shader_proj(vr3dcamera* cam);
	void vr_3d_renderer_draw_stereo_shader_proj(vr3dscene* scene);


	void _draw_framebuffers_on_planes_shader_proj(vr3dcamera* cam);

#ifdef HAVE_OPENHMD
	bool vr_3d_renderer_stereo_init_from_hmd(vr3dhmd* hmd);
#endif

private:
	void _create_fbo(GLuint* fbo, GLuint* color_tex, int width, int height);
	void _draw_eye(GLuint fbo, vr3dscene* scene, glm::mat4* mvp, glm::vec3* color);
	void _draw_framebuffers_on_planes();

public:
	vr3dmesh* render_plane;
	vr3dshader* shader;

	GLuint left_color_tex, left_fbo;
	GLuint right_color_tex, right_fbo;

	int eye_width;
	int eye_height;

	float filter_aspect;

	vrEncodeH264* eh264;
};



#endif // !_VR_3D_RENDERER_H_