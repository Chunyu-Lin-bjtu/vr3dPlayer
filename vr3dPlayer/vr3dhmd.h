#ifndef _VR_3D_HMD_H_
#define _VR_3D_HMD_H_

#include "common.h"
#include <openhmd/openhmd.h>

class vr3dhmd
{
public:
	vr3dhmd();
	~vr3dhmd();

public:
    //通过openhmd打开hmd
    void vr_3d_hmd_open_device();

    void vr_3d_hmd_reset();

    void vr_3d_hmd_get_device_properties();

    glm::mat4 vr_3d_hmd_get_matrix(ohmd_float_value type);

    glm::quat vr_3d_hmd_get_quaternion();

    void vr_3d_hmd_update();

    void vr_3d_hmd_eye_sep_inc();

    void vr_3d_hmd_eye_sep_dec();

    int vr_3d_hmd_get_eye_width();

    int vr_3d_hmd_get_eye_height();

    float vr_3d_hmd_get_screen_aspect();

    float vr_3d_hmd_get_eye_aspect();

public:
    ohmd_device* device;
    ohmd_context* hmd_context;

    int screen_width;
    int screen_height;

    float left_fov;
    float left_aspect;
    float right_fov;
    float right_aspect;

    float interpupillary_distance;
    float zfar;
    float znear;

    float eye_separation;
};
#endif // !_VR_3D_HMD_H_

