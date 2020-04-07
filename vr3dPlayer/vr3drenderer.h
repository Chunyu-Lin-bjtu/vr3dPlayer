#ifndef _VR_3D_RENDERER_H_
#define _VR_3D_RENDERER_H_

#include "common.h"

// 引入GLM库
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class vr3drenderer
{
public:
	vr3drenderer();
	~vr3drenderer();

private:
	void _draw_eye(GLuint fbo, /*vr3dscene* scene,*/ glm::mat4* mvp);

public:
	int eye_width;
	int eye_height;
};



#endif // !_VR_3D_RENDERER_H_