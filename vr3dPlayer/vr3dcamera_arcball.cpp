#include "vr3dcamera_arcball.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "vr3devent.h"

vr3dcamera_arcball::vr3dcamera_arcball()
{
    this->center_distance = 2.5;//眼睛在半径=center_distance的球面上移动，一直看向原点
    this->scroll_speed = 0.03;
    this->rotation_speed = 0.002;

    this->theta = 90.f / 180.f * M_PI;  //90.0 与y轴的夹角，与center_distance结合用于确定眼睛的位置
    this->phi = 90.f / 180.f * M_PI;    //0.00 与x轴的夹角，与center_distance结合用于确定眼睛的位置
    //this->theta = 90.f;  //90.0 与y轴的夹角，与center_distance结合用于确定眼睛的位置
    //this->phi = 90.f;    //0.00 与x轴的夹角，与center_distance结合用于确定眼睛的位置

    this->zoom_step = 0.95;
    this->min_fov = 45;
    this->max_fov = 110;
    // 设置camera 事件
    vr3devent::vr_3d_event_set_camera_func(this, &vr3dcamera::vr_3d_camera_navigation_event);
}

vr3dcamera_arcball::~vr3dcamera_arcball()
{
}

void vr3dcamera_arcball::vr_3d_camera_update_view()
{
    float radius = exp(this->center_distance);

    //printf("arcball radius = %f fov %f\n", radius, fov);
    
    //this->phi，x方向的偏移  this->theta，y方向的偏移
    eye = glm::vec3(radius * glm::radians(cos(this->theta)) * glm::radians(cos(this->phi)),
        radius * glm::radians(sin(this->theta)),
        radius * glm::radians(cos(this->theta)) * glm::radians(sin(this->phi)));

    glm::vec3 forward = center - eye;
    this->side = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
    this->up = glm::normalize(glm::cross(this->side, forward));
    
    // 投影变换矩阵
    glm::mat4 projection_matrix = glm::perspective(fov, aspect, znear, zfar);
    // 视变换矩阵
    glm::mat4 view_matrix = glm::lookAt(eye, center, up);

    //mvp = projection_matrix * view_matrix;
    mvp = view_matrix;

}

void vr3dcamera_arcball::vr_3d_camera_navigation_event(vr3devent* e)
{
    eventType event_type = e->vr_3d_event_get_type();

    switch (event_type) {
    case VR_3D_EVENT_MOUSE_MOVE: { //光标移动时处理
        /* hanlde the mouse motion for zooming and rotating the view */
        double x, y;
        x = e->vr_3d_event_get_double("pointer_x");
        y = e->vr_3d_event_get_double("pointer_y");

        double dx, dy;
        dx = x - this->cursor_last_x;
        //dy = y - this->cursor_last_y;
        dy = this->cursor_last_y - y;

        if (this->pressed_mouse_button != 0) {
            printf("Rotating [%fx%f]\n", dx, dy);
            vr_3d_camera_arcball_rotate(dx, dy);
        }
        this->cursor_last_x = x;
        this->cursor_last_y = y;
        break;
    }
    case VR_3D_EVENT_MOUSE_BUTTON_RELEASE: {
        int button;
        button = e->vr_3d_event_get_int("button");
        this->pressed_mouse_button = 0;

        if (button == 1) {
            /* first mouse button release */
            this->cursor_last_x = e->vr_3d_event_get_double("pointer_x");
            this->cursor_last_y = e->vr_3d_event_get_double("pointer_y");
        }
        else if (button == 4 || button == 6) {
            /* wheel up */
            //gst_3d_camera_arcball_translate (self, -1.0);
            if (this->fov > this->min_fov) {
                this->fov *= this->zoom_step;
                this->fov = std::fmax(this->min_fov, this->fov);
                vr_3d_camera_update_view();
            }
        }
        else if (button == 5 || button == 7) {
            /* wheel down */
            //gst_3d_camera_arcball_translate (self, 1.0);
            if (this->fov < this->max_fov) {
                this->fov /= this->zoom_step;
                this->fov = std::fmin(this->max_fov, this->fov);
                vr_3d_camera_update_view();
            }
        }
        break;
    }
    case VR_3D_EVENT_MOUSE_BUTTON_PRESS: {
        int button;
        button = e->vr_3d_event_get_int("button");
        this->pressed_mouse_button = 1;
        break;
    }
    default:
        break;
    }
}

void vr3dcamera_arcball::vr_3d_camera_arcball_translate(float z)
{
    float new_val = this->center_distance + z * this->scroll_speed;

    this->center_distance = std::fmax(0, new_val);

    printf("center distance: %f\n", this->center_distance);
    vr_3d_camera_update_view();
}

void vr3dcamera_arcball::vr_3d_camera_arcball_rotate(float x, float y)
{
    float delta_theta = y * this->rotation_speed;
    float delta_phi = x * this->rotation_speed;

    this->phi += delta_phi;
    this->theta += delta_theta;
    /* 2π < θ < π to avoid gimbal lock */
    /*float next_theta_pi = (this->theta + delta_theta) / M_PI;
    if (next_theta_pi < 2.0 && next_theta_pi > 1.0)
        this->theta += delta_theta;*/

    //printf("θ = %fπ ϕ = %fπ ", this->theta / M_PI, this->phi / M_PI);
    printf("thete = %f phi = %f", this->theta, this->phi);
    vr_3d_camera_update_view();
}
