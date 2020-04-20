#pragma once

// “˝»ÎGLMø‚
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/quaternion.hpp>

class vr3dmath
{
public:
	vr3dmath();
	~vr3dmath();
public:
	static void vr_3d_math_matrix_negate_component(const glm::mat4 * matrix, int n, int m, glm::mat4 * result);
	static glm::mat4 vr_3d_math_matrix_hadamard_product(const glm::mat4 * a, const glm::mat4 * b);
	static void vr_3d_math_vec3_print(const char * name, glm::vec3 * vector);
	static void vr_3d_math_vec3_negate(const glm::vec3 * vector, glm::vec3 * result);
};
