#ifndef _COMMON_H_
#define _COMMON_H_

// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include "GL/glew.h"

// 引入GLFW库
#include "GLFW/glfw3.h"

// 引入GLM库
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32sd.lib")
#pragma comment(lib, "SOIL.lib")


//#define HAVE_OPENHMD

#endif // _COMMON_H_

