#ifndef _VR_3D_CAMERA_ARCBALL_H_
#define _VR_3D_CAMERA_ARCBALL_H_
#include "common.h"
#include "vr3dcamera.h"


class vr3dcamera_arcball : public vr3dcamera
{
public:
	vr3dcamera_arcball();
	~vr3dcamera_arcball();

public:
    virtual void vr_3d_camera_update_view();

    void vr_3d_camera_arcball_translate(float z);

    void vr_3d_camera_arcball_rotate(float x, float y);

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

