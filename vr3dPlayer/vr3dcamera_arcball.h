﻿#ifndef _VR_3D_CAMERA_ARCBALL_H_
#define _VR_3D_CAMERA_ARCBALL_H_
#include "common.h"
#include "vr3dcamera.h"


class vr3dcamera_arcball : public vr3dcamera
{
public:
	vr3dcamera_arcball();
	~vr3dcamera_arcball();

private:
    float center_distance;
    float scroll_speed;
    double rotation_speed;
    float theta;
    float phi;

    float zoom_step;
    float min_fov;
    float max_fov;
};



#endif // !_VR_3D_CAMERA_ARCBALL_H_
