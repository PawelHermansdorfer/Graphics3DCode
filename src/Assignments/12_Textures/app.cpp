#include "app.h"

#include <Engine/camera.h>
#include <Engine/camera_controller.h>
#include <Engine/Mesh.h>
#include <Engine/KdMaterial.h>

#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include <glm/gtc/type_ptr.hpp>

#include "stb/stb_image.h"

void
SimpleShapeApplication::init()
{
    ////////////////////////////////////////
    // Camera
    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));

    ////////////////////////////////////////
    // Materials
    xe::KdMaterial::init();
    auto kd_white_material  = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1);
    auto kd_grey_material   = new xe::KdMaterial(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 1);
    auto kd_red_material    = new xe::KdMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 1);
    auto kd_green_material  = new xe::KdMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1);
    auto kd_blue_material   = new xe::KdMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1);
    auto kd_yellow_material = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1);

    auto [w, h] = frame_buffer_size();


    ////////////////////////////////////////
    // Texture
    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    GLuint tex_handle;
    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    auto kd_tex_material = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0, tex_handle);

    stbi_image_free(img);

    ////////////////////////////////////////
    // Vertex data
    std::vector<GLfloat> vertices = {
        -0.5f, -0.5f, 0.0f, 0.191f, 0.5f,
         0.5f, -0.5f, 0.0f, 0.5f,   0.191f,
         0.5f,  0.5f, 0.0f, 0.809f, 0.5f,
        -0.5f,  0.5f, 0.0f, 0.5f,   0.809f,

        0.0f,  0.0f, 1.0f, 0, 1, // red
        0.0f,  0.0f, 1.0f, 1, 1, // blue
        0.0f,  0.0f, 1.0f, 0, 0, // green
        0.0f,  0.0f, 1.0f, 1, 0, // yellow
    };

    std::vector<GLubyte> indices = {
        3, 1, 0,
        3, 2, 1,

        4, 3, 0,
        5, 2, 3,

        2, 7, 1,
        0, 1, 6,
    };

    auto pyramid = new xe::Mesh(5 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                                 indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(float),  vertices.data());
    pyramid->load_indices(0,  indices.size() * sizeof(GLubyte), indices.data());
    pyramid->add_attribute(xe::AttributeType::POSITION,    3, GL_FLOAT, 0);
    pyramid->add_attribute(xe::AttributeType::TEXCOORD_0,  2, GL_FLOAT, 3 * sizeof(GLfloat));

    pyramid->add_primitive(0, 18, new xe::KdMaterial({1.f, 1.f, 1.0f, 1.0f}, 0, tex_handle));

    add_mesh(pyramid);

    ////////////////////////////////////////
    camera()->look_at(glm::vec3(2.0f,1.0f,2.0f),
                      glm::vec3(0.0f,0.0f,0.0f),
                      glm::vec3(0.0f,0.0f,1.0f));

    float aspect = static_cast<float>(w) / static_cast<float>(h);
    camera()->perspective(glm::radians(45.0f), aspect, 0.1f, 20.0f);

    M_ = glm::mat4(1.0f);

    glCreateBuffers(1, &u_trans_buffer_handle_);
    glNamedBufferData(u_trans_buffer_handle_, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);

    ////////////////////////////////////////
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}


void
SimpleShapeApplication::frame()
{
    glm::mat4 P = camera()->projection();
    glm::mat4 V = camera()->view();
    glm::mat4 PVM = P * V * M_;

    glNamedBufferSubData(u_trans_buffer_handle_, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_);

    for(auto m: meshes_) m->draw();

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0);
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
