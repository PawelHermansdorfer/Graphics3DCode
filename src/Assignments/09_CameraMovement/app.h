//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "glad/gl.h"
#include <glm/glm.hpp>

#include "Application/application.h"

#include "camera.h"
#include "camera_controller.h"


class SimpleShapeApplication : public xe::Application {
    public:
        SimpleShapeApplication(int width, int height, std::string title, bool debug, int swap_interval = 1) : Application(
                                                                                                                          width, height, title,
                                                                                                                          debug, swap_interval), camera_(nullptr) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;
    void scroll_callback(double xoffset, double yoffset) override;
    void mouse_button_callback(int button, int action, int mods) override;
    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() const { assert(camera_); return camera_; }


    void set_controler(CameraController *controller) { controller_ = controller; }


    private:
        GLuint vao_;
        GLuint u_trans_buffer_handle_;

        glm::mat4 M_;

        Camera *camera_;
        CameraController *controller_;
};
