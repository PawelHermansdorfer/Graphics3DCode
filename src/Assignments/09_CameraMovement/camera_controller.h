#pragma once

#include "camera.h"

class CameraController {
public:
    CameraController()
        : camera_(nullptr)
        , scale_(0.005f)
        , LMB_pressed_(false)
        , x_(0.0f)
        , y_(0.0f)
    {}

    CameraController(Camera *camera)
        : camera_(camera)
        , scale_(0.005f)
        , LMB_pressed_(false)
        , x_(0.0f)
        , y_(0.0f)
    {}

    void set_camera(Camera *camera) { camera_ = camera; }

    void rotate_camera(float dx, float dy)
    {
        if (!camera_) return;
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    void mouse_moved(float x, float y)
    {
        if (LMB_pressed_)
        {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    }

    void LMB_pressed(float x, float y)
    {
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    }

    void LMB_released(float x, float y)
    {
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    }

private:
    Camera *camera_;

    float scale_;
    bool LMB_pressed_;
    float x_, y_;
};
