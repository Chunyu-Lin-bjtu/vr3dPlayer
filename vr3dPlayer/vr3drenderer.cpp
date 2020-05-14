#include "vr3drenderer.h"
#include "vr3drenderer.h"
#include "vr3dhmd.h"
#include "vr3dcamera_hmd.h"
#include "vr3dscene.h"
#include "vr3dtexture.h"

#include "vrencode.h"

vr3drenderer::vr3drenderer()
{
	this->shader = nullptr;
	this->left_color_tex = 0;
	this->left_fbo = 0;
	this->right_color_tex = 0;
	this->right_fbo = 0;
	this->eye_width = 1;
	this->eye_height = 1;
	this->filter_aspect = 1.0f;

	eh264 = nullptr;
}

vr3drenderer::~vr3drenderer()
{
	if (eh264) {
		delete eh264;
		eh264 = nullptr;
	}
}

#ifdef HAVE_OPENHMD
bool vr3drenderer::vr_3d_renderer_stereo_init_from_hmd(vr3dhmd* hmd)
{
	if (!hmd->device) {
		return false;
	}
	filter_aspect = hmd->vr_3d_hmd_get_eye_aspect();
	//filter_aspect = hmd->vr_3d_hmd_get_screen_aspect();
#if 0
	eye_width = hmd->vr_3d_hmd_get_eye_width();
	eye_height = hmd->vr_3d_hmd_get_eye_height();
#else
	eye_width = 960;
	eye_height = 540;

	eh264 = new vrEncodeH264(eye_width, eye_height, 60);
	eh264->vr_encode_h264_from_framebuffer_map(2, "rgb.mp4");

#endif // 0
	
	
	printf("gst_3d_renderer_stereo_init_from_hmd eye_width(%d) eye_height(%d) filter_aspect(%f) .\n",
		eye_width, eye_height, filter_aspect);

	return true;
}
#endif

void vr3drenderer::_create_fbo(GLuint* fbo, GLuint* color_tex, int width, int height)
{
	glGenFramebuffers(1, fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	// 附加 color attachment
	*color_tex = TextureHelper::makeAttachmentTexture(0, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE); // 创建FBO中的纹理
	
	// 附加 depth stencil RBO attachment
	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // 预分配内存
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);//将深度模板缓冲区使用RBO代替
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *color_tex, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("failed to create fbo %x\n", status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void vr3drenderer::_draw_eye(GLuint fbo, vr3dscene* scene, glm::mat4* mvp, glm::vec3* color)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, this->eye_width, this->eye_height);
	glClearColor(color->r, color->g, color->b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->vr_3d_scene_draw_nodes(mvp);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
}


void vr3drenderer::_draw_framebuffers_on_planes()
{
	glm::mat4 projection_ortho;
	projection_ortho = glm::ortho(-filter_aspect, filter_aspect, 1.0f, -1.0f, -1.0f, 1.0f);
	//projection_ortho = glm::mat4();
	shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(projection_ortho));
	render_plane->vr_3d_mesh_bind();

	/* left framebuffer */
	glViewport(0, 0, eye_width/2, eye_height);
	glBindTexture(GL_TEXTURE_2D, left_color_tex);
	render_plane->vr_3d_mesh_draw();

	/* right framebuffer */
	glViewport(eye_width/2, 0, eye_width/2, eye_height);
	glBindTexture(GL_TEXTURE_2D, right_color_tex);
	render_plane->vr_3d_mesh_draw();

	eh264->vr_encode_h264_run_framebuffer_map();
}

void vr3drenderer::_draw_framebuffers_on_planes_shader_proj(vr3dcamera* cam)
{
	vr3dcamera_hmd* hmd_cam = (vr3dcamera_hmd*)(cam);

	glm::mat4 projection_ortho;
	projection_ortho = glm::ortho(-this->filter_aspect, this->filter_aspect, 1.0f, -1.0f, -1.0f, 1.0f);
	this->shader->vr_3d_shader_upload_matrix("mvp", glm::value_ptr(projection_ortho));

	this->shader->vr_3d_shader_upload_matrix("vp", glm::value_ptr(hmd_cam->left_vp_matrix));

	this->render_plane->vr_3d_mesh_bind();

	/* left framebuffer */
	glViewport(0, 0, this->eye_width, this->eye_height);
	this->render_plane->vr_3d_mesh_draw();

	this->shader->vr_3d_shader_upload_matrix("vp", glm::value_ptr(hmd_cam->right_vp_matrix));

	/* right framebuffer */
	glViewport(this->eye_width, 0, this->eye_width, this->eye_height);
	this->render_plane->vr_3d_mesh_draw();
}

void vr3drenderer::vr_3d_renderer_init_stereo(vr3dcamera* cam)
{
	vr3dcamera_hmd* hmd_cam = (vr3dcamera_hmd*)(cam);
	vr3dhmd* hmd = hmd_cam->hmd;
	float aspect_ratio = hmd->left_aspect;

	render_plane = new vr3dmesh();
	render_plane->vr_3d_mesh_new_plane();

	shader = new vr3dshader("shader/mvp_uv.vert", "shader/texture_equirectangular_sphere.frag");

	if (shader == NULL) {
		printf("Failed to create shaders. \n");
		return;
	}

	render_plane->vr_3d_mesh_bind_shader(shader);

	_create_fbo(&left_fbo, &left_color_tex, eye_width, eye_height);
	_create_fbo(&right_fbo, &right_color_tex, eye_width, eye_height);

	shader->vr_3d_shader_bind();
	shader->vr_3d_shader_update_uniform_1i("texture", 0);
}

void vr3drenderer::vr_3d_renderer_init_stereo_shader_proj(vr3dcamera* cam)
{
	vr3dcamera_hmd* hmd_cam = (vr3dcamera_hmd*)(cam);
	vr3dhmd* hmd = hmd_cam->hmd;
	float aspect_ratio = hmd->left_aspect;

	this->render_plane = new vr3dmesh();
	render_plane->vr_3d_mesh_new_plane();

	this->shader = new vr3dshader("shader/mvp_uv.vert", "shader/texture_equirectangular_sphere.frag");

	if (this->shader == NULL) {
		printf("Failed to create shaders. \n");
		return;
	}

	this->render_plane->vr_3d_mesh_bind_shader(this->shader);

	this->shader->vr_3d_shader_bind();
	this->shader->vr_3d_shader_update_uniform_1i("texture", 0);
}

void vr3drenderer::vr_3d_renderer_draw_stereo(vr3dscene* scene)
{
	vr3dcamera_hmd* hmd_cam = (vr3dcamera_hmd*)(scene->camera);

#if 1
	/* left eye */
	_draw_eye(left_fbo, scene, &hmd_cam->left_vp_matrix, &(glm::vec3(0.18f, 0.04f, 0.14f)));

	/* right eye */
	_draw_eye(right_fbo, scene, &hmd_cam->right_vp_matrix, &(glm::vec3(0.04f, 0.18f, 0.14f)));
#else
	glm::mat4 identiy = glm::mat4();	//TODO...先更新一个单位矩阵
	_draw_eye(left_fbo, scene, &identiy, &(glm::vec3(0.18f, 0.04f, 0.14f)));
	_draw_eye(right_fbo, scene, &identiy, &(glm::vec3(0.04f, 0.18f, 0.14f)));
#endif // 0

	scene->vr_3d_scene_clear_state();

	shader->vr_3d_shader_bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_draw_framebuffers_on_planes();
	scene->vr_3d_scene_clear_state();
}

void vr3drenderer::vr_3d_renderer_draw_stereo_shader_proj(vr3dscene* scene)
{
	shader->vr_3d_shader_bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_draw_framebuffers_on_planes_shader_proj(scene->camera);
}
