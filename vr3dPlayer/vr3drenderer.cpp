#include "vr3drenderer.h"

vr3drenderer::vr3drenderer()
{
}

vr3drenderer::~vr3drenderer()
{
}


void vr3drenderer::_draw_eye(GLuint fbo, /*vr3dscene* scene,*/ glm::mat4* mvp)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, this->eye_width, this->eye_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
