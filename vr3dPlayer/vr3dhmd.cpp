
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define OHMD_STATIC
#include "vr3dhmd.h"
#pragma comment(lib, "openhmd.lib")
#pragma comment(lib, "hidapi.lib")
vr3dhmd::vr3dhmd()
{
    device = nullptr;
    hmd_context = nullptr;

    screen_width = 0;
    screen_height = 0;

    left_fov = 0;
    left_aspect = 0;
    right_fov = 0;
    right_aspect = 0;

    interpupillary_distance = 0;
    zfar = 0;
    znear = 0;

    eye_separation = 0;

    vr_3d_hmd_open_device();
    if (device)
        vr_3d_hmd_get_device_properties();
}

vr3dhmd::~vr3dhmd()
{
}

void vr3dhmd::vr_3d_hmd_open_device()
{
    int picked_device = 0;
    hmd_context = ohmd_ctx_create();
    if (!hmd_context) return;

    int num_devices = ohmd_ctx_probe(hmd_context);
    if (num_devices < 0) {
        printf("Failed to probe devices: %s\n", ohmd_ctx_get_error(hmd_context));
        return;
    }

    printf("We have %d devices.\n", num_devices);

    for (int i = 0; i < num_devices; i++) {
        printf("device %d", i);
        printf("  vendor:  %s", ohmd_list_gets(hmd_context, i, OHMD_VENDOR));
        printf("  product: %s", ohmd_list_gets(hmd_context, i, OHMD_PRODUCT));
        printf("  path:    %s", ohmd_list_gets(hmd_context, i, OHMD_PATH));
    }

    device = ohmd_list_open_device(hmd_context, picked_device);

    if (!device) {
        printf("Failed to open device: %s\n", ohmd_ctx_get_error(hmd_context));
        printf("  vendor:  %s", ohmd_list_gets(hmd_context, picked_device, OHMD_VENDOR));
        printf("  product: %s", ohmd_list_gets(hmd_context, picked_device, OHMD_PRODUCT));
        printf("  path:    %s", ohmd_list_gets(hmd_context, picked_device, OHMD_PATH));
        printf("Make sure you have access rights and a working rules "
            "file for your headset in /usr/lib/udev/rules.d");
        return;
    }
}

void vr3dhmd::vr_3d_hmd_reset()
{
    /* reset rotation and position */
    float zero[] = { 0, 0, 0, 1 };
    ohmd_device_setf(device, OHMD_ROTATION_QUAT, zero);
    ohmd_device_setf(device, OHMD_POSITION_VECTOR, zero);
    printf("Resetting OHMD_ROTATION_QUAT and OHMD_POSITION_VECTOR.\n");
}

void vr3dhmd::vr_3d_hmd_get_device_properties()
{
    ohmd_device_geti(device, OHMD_SCREEN_HORIZONTAL_RESOLUTION, &screen_width);
    ohmd_device_geti(device, OHMD_SCREEN_VERTICAL_RESOLUTION, &screen_height);

    printf("HMD screen resolution: %dx%d", screen_width, screen_height);

    float screen_width_physical;
    float screen_height_physical;

    ohmd_device_getf(device, OHMD_SCREEN_HORIZONTAL_SIZE, &screen_width_physical);
    ohmd_device_getf(device, OHMD_SCREEN_VERTICAL_SIZE, &screen_height_physical);

    printf("Physical HMD screen dimensions: %.3fx%.3fcm",
        screen_width_physical * 100.0, screen_height_physical * 100.0);

    float x_pixels_per_cm =
        (float)screen_width / (screen_width_physical * 100.0);
    float y_pixels_per_cm =
        (float)screen_height / (screen_height_physical * 100.0);

    const float inch = 2.54;
    printf("HMD DPI %f x %f", x_pixels_per_cm / inch, y_pixels_per_cm / inch);

    float lens_x_separation;
    float lens_y_position;

    ohmd_device_getf(device, OHMD_LENS_HORIZONTAL_SEPARATION, &lens_x_separation);
    ohmd_device_getf(device, OHMD_LENS_VERTICAL_POSITION, &lens_y_position);

    printf("Horizontal Lens Separation: %.3fcm", lens_x_separation * 100.0);
    printf("Vertical Lens Position: %.3fcm", lens_y_position * 100.0);

    ohmd_device_getf(device, OHMD_LEFT_EYE_FOV, &left_fov);
    ohmd_device_getf(device, OHMD_RIGHT_EYE_FOV, &right_fov);
    printf("FOV (left/right): %f %f", left_fov, right_fov);

    ohmd_device_getf(device, OHMD_LEFT_EYE_ASPECT_RATIO, &left_aspect);
    ohmd_device_getf(device, OHMD_RIGHT_EYE_ASPECT_RATIO, &right_aspect);
    printf("Aspect Ratio (left/right): %f %f", left_aspect, right_aspect);

    ohmd_device_getf(device, OHMD_EYE_IPD, &interpupillary_distance);
    printf("interpupillary_distance %.3fcm", interpupillary_distance * 100.0);

    //self->eye_separation = interpupillary_distance * 100.0 / 2.0;
    //self->eye_separation = 0.65;
    eye_separation = 6.5;

    ohmd_device_getf(device, OHMD_PROJECTION_ZNEAR, &znear);
    ohmd_device_getf(device, OHMD_PROJECTION_ZFAR, &zfar);
    printf("znear %f, zfar %f", znear, zfar);

    float kappa[6];
    ohmd_device_getf(device, OHMD_DISTORTION_K, kappa);
    printf("Kappa: %f, %f, %f, %f, %f, %f",
        kappa[0], kappa[1], kappa[2], kappa[3], kappa[4], kappa[5]);

    float position[3];
    ohmd_device_getf(device, OHMD_POSITION_VECTOR, position);
    printf("position: %f, %f, %f", position[0], position[1], position[2]);
}


glm::mat4 vr3dhmd::vr_3d_hmd_get_matrix(ohmd_float_value type)
{
    float matrix[16];
    ohmd_device_getf(this->device, type, matrix);
    glm::mat4 hmd_matrix = glm::make_mat4(matrix);

    return hmd_matrix;
}

glm::quat vr3dhmd::vr_3d_hmd_get_quaternion()
{
    float quaternion[4];// x,y,z,w
    ohmd_device_getf(this->device, OHMD_ROTATION_QUAT, quaternion);
    glm::quat hmd_quat(quaternion[3], quaternion[0], -quaternion[1], quaternion[2]);

    return hmd_quat;
}

void vr3dhmd::vr_3d_hmd_update()
{
    if (!this->hmd_context) return;
    if (!this->device) return;
    ohmd_ctx_update(this->hmd_context);
    return;
}

void vr3dhmd::vr_3d_hmd_eye_sep_inc()
{
    this->eye_separation += 1.f;
    printf("separation: %f", this->eye_separation);
}

void vr3dhmd::vr_3d_hmd_eye_sep_dec()
{
    this->eye_separation -= 1.f;
    printf("separation: %f", this->eye_separation);
}

int vr3dhmd::vr_3d_hmd_get_eye_width()
{
    return this->screen_width / 2;
}

int vr3dhmd::vr_3d_hmd_get_eye_height()
{
    return this->screen_height;
}

float vr3dhmd::vr_3d_hmd_get_screen_aspect()
{
    int w = this->screen_width;
    int h = this->screen_height;
    return (float)w / (float)h;
}

float vr3dhmd::vr_3d_hmd_get_eye_aspect()
{
    return (float)vr_3d_hmd_get_eye_width() / (float)vr_3d_hmd_get_eye_height();
}