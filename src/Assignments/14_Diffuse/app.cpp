#include "app.h"

#include <Engine/camera.h>
#include <Engine/camera_controller.h>
#include <Engine/Mesh.h>
#include <Engine/KdMaterial.h>
#include <Engine/BlinnPhongMaterial.h>
#include <Engine/texture.h>
#include <Engine/mesh_loader.h>
#include <Engine/light.h>

#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include <glm/gtc/type_ptr.hpp>

#include "stb/stb_image.h"

void
SimpleShapeApplication::init()
{
    auto [w, h] = frame_buffer_size();

    ////////////////////////////////////////
    // Camera
    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));


    ////////////////////////////////////////
    // Materials
    xe::KdMaterial::init();
    xe::BlinnPhongMaterial::init();

    ////////////////////////////////////////
    // Mesh
    auto mesh = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(mesh);

    ////////////////////////////////////////
    camera()->look_at(glm::vec3(0.0f,0.0f,3.0f),
                      glm::vec3(0.0f,0.0f,0.0f),
                      glm::vec3(0.0f,1.0f,0.0f));

    float aspect = static_cast<float>(w) / static_cast<float>(h);
    camera()->perspective(glm::radians(45.0f), aspect, 0.1f, 20.0f);

    M_ = glm::mat4(1.0f);

    glCreateBuffers(1, &u_trans_buffer_handle_);
    glNamedBufferData(u_trans_buffer_handle_, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), 0, GL_DYNAMIC_DRAW);


    add_light(xe::PointLight(glm::vec3(0, 0, 1.0f), glm::vec3(1,1,1), 1.0f, 0.1f));
    glCreateBuffers(1, &u_light_buffer_handle_);
    glNamedBufferData(u_light_buffer_handle_, sizeof(glm::vec3) + sizeof(int) + xe::MAX_POINT_LIGHTS * sizeof(xe::PointLight), 0, GL_DYNAMIC_DRAW);

    ////////////////////////////////////////
    glClearColor(0.81f, 0.81f, 0.81f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
}


void
SimpleShapeApplication::frame()
{
    glm::mat4 P = camera()->projection();
    glm::mat4 V = camera()->view();
    glm::mat4 PVM = P * V * M_;
    glm::mat4 VM = V * M_;

    auto R = glm::mat3(VM);
    auto VM_Normal = glm::mat3(glm::cross(R[1], R[2]),
                               glm::cross(R[2], R[0]),
                               glm::cross(R[0], R[1]));

    glNamedBufferSubData(u_trans_buffer_handle_, 0,                   sizeof(glm::mat4), &PVM[0]);
    glNamedBufferSubData(u_trans_buffer_handle_, sizeof(glm::mat4),   sizeof(glm::mat4), &VM[0]);
    int normal_offset = 2 * sizeof(glm::mat4);
    for (int k = 0; k < 3; ++k)
    {
        glNamedBufferSubData(u_trans_buffer_handle_, normal_offset + k * sizeof(glm::vec4), sizeof(glm::vec3), &VM_Normal[k]);
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_);


    glm::vec3 ambient = glm::vec3(0.25f, 0.25f, 0.25f);
    int lights_size = lights_.size();
    glNamedBufferSubData(u_light_buffer_handle_, 0,                 sizeof(glm::vec3), &ambient[0]);
    glNamedBufferSubData(u_light_buffer_handle_, sizeof(glm::vec3), sizeof(int),       &lights_size);
    for (int i = 0; i < lights_size; ++i) {
        xe::PointLight light_vs = xe::transform(lights_[i], VM);
        glNamedBufferSubData(u_light_buffer_handle_,
                             sizeof(glm::vec3) + sizeof(int) + i * sizeof(xe::PointLight),
                             sizeof(xe::PointLight),
                             &light_vs);
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_handle_);


    for(auto m: meshes_) m->draw();


    glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, 0);
}


void
SimpleShapeApplication::framebuffer_resize_callback(int w, int h)
{
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;
    camera()->set_aspect(aspect);
}


void
SimpleShapeApplication::scroll_callback(double xoffset, double yoffset)
{
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 20.0f);
}


void
SimpleShapeApplication::mouse_button_callback(int button, int action, int mods)
{
    Application::mouse_button_callback(button, action, mods);

    if (controller_)
    {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed(x, y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released(x, y);
    }
}


void
SimpleShapeApplication::cursor_position_callback(double x, double y)
{
    Application::cursor_position_callback(x, y);
    if (controller_)
    {
        controller_->mouse_moved(x, y);
    }
}
