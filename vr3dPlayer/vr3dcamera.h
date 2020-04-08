#ifndef _VR_3D_CAMERA_H_
#define _VR_3D_CAMERA_H_
#include <list>

#include "common.h"

using namespace std;

class vr3dcamera
{
public:
	vr3dcamera();
	~vr3dcamera();

public:
    virtual void vr_3d_camera_update_view() = 0;

    void vr_3d_camera_update_view_mvp();

public:
    glm::mat4 mvp;

    /* camera position */
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;

    list<int> pushed_buttons;

    /* perspective */
    float fov;
    float aspect;
    float znear;
    float zfar;
    bool ortho;

    /* user input */
    double cursor_last_x;
    double cursor_last_y;

    int pressed_mouse_button;
};

#endif // !_VR_3D_CAMERA_H_

