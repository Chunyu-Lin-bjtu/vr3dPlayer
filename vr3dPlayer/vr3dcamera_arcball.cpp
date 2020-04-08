#include "vr3dcamera_arcball.h"
#define _USE_MATH_DEFINES
#include <math.h>


vr3dcamera_arcball::vr3dcamera_arcball()
{
    this->center_distance = 2.5;//眼睛在 半径=center_distance 的球面上移动，一直看向原点
    this->scroll_speed = 0.03;
    this->rotation_speed = 0.002;

    this->theta = 90.f / 180.f * M_PI;  //90.0 与y轴的夹角，与center_distance结合用于确定眼睛的位置
    this->phi = 90.f / 180.f * M_PI;    //0.00 与x轴的夹角，与center_distance结合用于确定眼睛的位置

    this->zoom_step = 0.95;
    this->min_fov = 45;
    this->max_fov = 110;
}

vr3dcamera_arcball::~vr3dcamera_arcball()
{
}

void vr3dcamera_arcball::vr_3d_camera_update_view()
{
    float radius = exp(this->center_distance);

    printf("arcball radius = %f fov %f", radius, fov);

    eye = glm::vec3( radius * sin(this->theta) * cos(this->phi),
        radius * -cos(this->theta),
        radius * sin(this->theta) * sin(this->phi));
    
    // 投影变换矩阵
    glm::mat4 projection_matrix = glm::perspective(fov, aspect, znear, zfar);
    // 视变换矩阵
    glm::mat4 view_matrix = glm::lookAt(eye, center, up);

    /* fix graphene look at */
   /* graphene_matrix_t v_inverted;
    graphene_matrix_t v_inverted_fix;
    graphene_matrix_inverse(&view_matrix, &v_inverted);
    gst_3d_math_matrix_negate_component(&v_inverted, 3, 2, &v_inverted_fix);*/
    mvp = view_matrix * projection_matrix;
    //graphene_matrix_multiply(&v_inverted_fix, &projection_matrix, &cam->mvp);
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

    /* 2π < θ < π to avoid gimbal lock */
    float next_theta_pi = (this->theta + delta_theta) / M_PI;
    if (next_theta_pi < 2.0 && next_theta_pi > 1.0)
        this->theta += delta_theta;

    printf("θ = %fπ ϕ = %fπ", this->theta / M_PI, this->phi / M_PI);
    vr_3d_camera_update_view();
}
