#include "vr3dmath.h"
#include <cstdio>

vr3dmath::vr3dmath()
{
}

vr3dmath::~vr3dmath()
{
}

void vr3dmath::vr_3d_math_matrix_negate_component(const glm::mat4 * matrix, int n, int m, glm::mat4 * result)
{
}

glm::mat4 vr3dmath::vr_3d_math_matrix_hadamard_product(const glm::mat4 * a, const glm::mat4 * b)
{
	auto ap = glm::value_ptr(*a);
	auto bp = glm::value_ptr(*b);
	float values[16];
	for (int x = 0; x < 16; x++) {
		values[x] = ap[x] * bp[x];
	}
	return glm::make_mat4(values);
	//graphene_matrix_init_from_float(result, values);
}

void vr3dmath::vr_3d_math_vec3_print(const char * name, glm::vec3 * vector)
{
	printf("%s %f %f %f\n", name, vector->x, vector->y, vector->z);
}

// È¡·´
void vr3dmath::vr_3d_math_vec3_negate(const glm::vec3 * vector, glm::vec3 * result)
{
	*result = glm::vec3() - *vector;
}