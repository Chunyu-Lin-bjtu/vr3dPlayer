#ifndef _COMMON_H_
#define _COMMON_H_

// ����GLEW�� ���徲̬����
#define GLEW_STATIC
#include "GL/glew.h"

// ����GLFW��
#include "GLFW/glfw3.h"

// ����GLM��
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32sd.lib")
#pragma comment(lib, "SOIL.lib")

#endif // _COMMON_H_
