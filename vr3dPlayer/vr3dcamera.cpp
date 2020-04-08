#include "vr3dcamera.h"

vr3dcamera::vr3dcamera()
{
    this->fov = 45.0;
    //this->aspect = 4.0 / 3.0;
    this->aspect = 16.0 / 9.0;
    this->znear = 0.01;
    this->zfar = 1000.0;

    this->eye = glm::vec3(0.f, 0.f, 1.f);
    this->center = glm::vec3(0.f, 0.f, 0.f);
    this->up = glm::vec3(0.f, 1.f, 0.f);

    this->cursor_last_x = 0;
    this->cursor_last_y = 0;
    this->pressed_mouse_button = 0;
}

vr3dcamera::~vr3dcamera()
{
}

void vr3dcamera::vr_3d_camera_update_view()
{
}

void vr3dcamera::vr_3d_camera_update_view_mvp()
{
    glm::mat4 projection_matrix;
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
    projection_matrix = glm::perspective(this->fov, this->aspect, this->znear, this->zfar);

    glm::mat4 view_matrix;
    view_matrix = glm::lookAt(this->eye, this->center, this->up);

    this->mvp = view_matrix * projection_matrix;
}