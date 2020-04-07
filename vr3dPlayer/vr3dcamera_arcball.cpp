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
