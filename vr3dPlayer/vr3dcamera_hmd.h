#ifndef _VR_3D_CAMERA_HMD_H_
#define _VR_3D_CAMERA_HMD_H_

#include "vr3dmath.h"
#include "vr3dhmd.h"
#include "vr3dcamera.h"

typedef enum Vr3DHmdQueryType
{
    HMD_QUERY_TYPE_QUATERNION_MONO,
    HMD_QUERY_TYPE_QUATERNION_STEREO,
    HMD_QUERY_TYPE_MATRIX_STEREO,
    HMD_QUERY_TYPE_NONE,
} Vr3DHmdQueryType;

class vr3dcamera_hmd;
//typedef void (*update_view_funct) (vr3dcamera_hmd*);

class vr3dcamera_hmd :public vr3dcamera
{
public:
	vr3dcamera_hmd();
	~vr3dcamera_hmd();
public:
    virtual void vr_3d_camera_update_view();
    virtual void vr_3d_camera_navigation_event(vr3devent* e);

    void vr_3d_camera_hmd_update_view_from_matrix();
    void vr_3d_camera_hmd_update_view_from_quaternion();
    void vr_3d_camera_hmd_update_view_from_quaternion_stereo();
private:
    glm::mat4 _matrix_invert_y_rotation(const glm::mat4* source);

public:
    glm::mat4 left_vp_matrix;
    glm::mat4 right_vp_matrix;

    Vr3DHmdQueryType query_type;

    vr3dhmd* hmd;
    //update_view_funct func;
};
#endif // !_VR_3D_CAMERA_HMD_H_

