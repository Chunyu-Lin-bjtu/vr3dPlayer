#include "vr3dcamera_hmd.h"

vr3dcamera_hmd::vr3dcamera_hmd()
{
	hmd = new vr3dhmd();
	query_type = HMD_QUERY_TYPE_MATRIX_STEREO;
}

vr3dcamera_hmd::~vr3dcamera_hmd()
{
}

void vr3dcamera_hmd::vr_3d_camera_update_view()
{
	hmd->vr_3d_hmd_update();
	vr_3d_camera_hmd_update_view_from_matrix();
}

void vr3dcamera_hmd::vr_3d_camera_navigation_event(vr3devent* e)
{
}

void vr3dcamera_hmd::vr_3d_camera_hmd_update_view_from_matrix()
{
	printf("gst_3d_camera_hmd_update_view_from_matrix.\n");
	glm::mat4 left_eye_model_view = hmd->vr_3d_hmd_get_matrix(OHMD_LEFT_EYE_GL_MODELVIEW_MATRIX);
	glm::mat4 left_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_LEFT_EYE_GL_PROJECTION_MATRIX);

	glm::mat4 right_eye_model_view = hmd->vr_3d_hmd_get_matrix(OHMD_RIGHT_EYE_GL_MODELVIEW_MATRIX);
	glm::mat4 right_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX);

	glm::mat4 left_eye_model_view_inv;
	glm::mat4 right_eye_model_view_inv;

	left_eye_model_view_inv = _matrix_invert_y_rotation(&left_eye_model_view);
	right_eye_model_view_inv = _matrix_invert_y_rotation(&right_eye_model_view);

	this->left_vp_matrix = left_eye_projection * right_eye_model_view_inv;
	this->right_vp_matrix = right_eye_projection * left_eye_model_view_inv;
}

void vr3dcamera_hmd::vr_3d_camera_hmd_update_view_from_quaternion()
{
	/* projection from OpenHMD */
	printf("gst_3d_camera_hmd_update_view_from_quaternion.\n");
	glm::mat4 left_eye_model_view;
	glm::mat4 left_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_LEFT_EYE_GL_PROJECTION_MATRIX);

	glm::mat4 right_eye_model_view;
	glm::mat4 right_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX);

	/* rotation from OpenHMD */
	glm::quat quat = hmd->vr_3d_hmd_get_quaternion();
	right_eye_model_view = glm::toMat4(quat);
	left_eye_model_view = glm::toMat4(quat);

	this->left_vp_matrix = left_eye_model_view * left_eye_projection;
	this->right_vp_matrix = right_eye_model_view * right_eye_projection;
}

void vr3dcamera_hmd::vr_3d_camera_hmd_update_view_from_quaternion_stereo()
{
	/* projection from OpenHMD */
	printf("gst_3d_camera_hmd_update_view_from_quaternion_stereo.\n");
	glm::mat4 left_eye_model_view;
	glm::mat4 left_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_LEFT_EYE_GL_PROJECTION_MATRIX);

	glm::mat4 right_eye_model_view;
	glm::mat4 right_eye_projection = hmd->vr_3d_hmd_get_matrix(OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX);

	/* rotation from OpenHMD */
	glm::quat quat = hmd->vr_3d_hmd_get_quaternion();
	right_eye_model_view = glm::toMat4(quat);
	left_eye_model_view = glm::toMat4(quat);

	/* eye separation */
	glm::vec3 left_eye(+this->hmd->eye_separation, 0, 0);
	glm::mat4 translate_left = glm::translate(glm::mat4(1.0f), left_eye);

	glm::vec3 rigth_eye(-this->hmd->eye_separation, 0, 0);
	glm::mat4 translate_right = glm::translate(glm::mat4(1.0f), rigth_eye);

	right_eye_model_view = right_eye_model_view * translate_right;
	left_eye_model_view = left_eye_model_view * translate_left;

	this->left_vp_matrix = left_eye_model_view * left_eye_projection;
	this->right_vp_matrix = right_eye_model_view * right_eye_projection;
}

glm::mat4 vr3dcamera_hmd::_matrix_invert_y_rotation(const glm::mat4* source)
{
	printf("_matrix_invert_y_rotation.\n");
	float invert_values[] = {
	   1, -1,  1, 1,
	  -1,  1, -1, 1,
	   1, -1,  1, 1,
	   1,  1,  1, 1
	};
	glm::mat4 invert_matrix = glm::make_mat4(invert_values);

	return vr3dmath::vr_3d_math_matrix_hadamard_product(&invert_matrix, source);
}
