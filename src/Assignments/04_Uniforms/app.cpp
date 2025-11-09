//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::pi

void SimpleShapeApplication::init() {
    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    std::vector<GLfloat> vertices = {
            -0.5f, +0.0f, +0.0f,    1.0f, 0.0f, 0.0f,
            +0.5f, +0.0f, +0.0f,    1.0f, 0.0f, 0.0f,
            +0.0f, +0.5f, +0.0f,    1.0f, 0.0f, 0.0f,

            -0.5f, +0.0f, +0.0f,    0.0f, 1.0f, 0.0f,
            +0.5f, +0.0f, +0.0f,    0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, +0.0f,    0.0f, 1.0f, 0.0f,
            +0.5f, -0.5f, +0.0f,    0.0f, 1.0f, 0.0f,
    };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        3, 4, 5,
        4, 5, 6,

    };


    ////////////////////////////////////////
    GLuint v_buffer_handle, i_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &v_buffer_handle));
    OGL_CALL(glNamedBufferData(v_buffer_handle, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));

    OGL_CALL(glCreateBuffers(1, &i_buffer_handle));
    OGL_CALL(glNamedBufferData(i_buffer_handle, indices.size() * sizeof(GLubyte),
                               indices.data(), GL_STATIC_DRAW));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));

    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));
    OGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(sizeof(GLfloat) * 3)));


    ////////////////////////////////////////
    // Mixer
    GLuint mixer_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &mixer_buffer_handle));

    OGL_CALL(glNamedBufferData(mixer_buffer_handle, 8 * sizeof(float), 0, GL_STATIC_DRAW));

    float strength = 0.5f;
    float mix_color[3] = {0.0f, 0.0f, 1.0f};

    float mixer_data[] = {
        strength, 0.0f, 0.0f, 0.0f,
        mix_color[0], mix_color[1], mix_color[2], 0.0f
    };

    OGL_CALL(glNamedBufferSubData(mixer_buffer_handle, 0, sizeof(mixer_data), mixer_data));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, mixer_buffer_handle));

    ////////////////////////////////////////
    // Transformations
    GLuint transform_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &transform_buffer_handle));

    OGL_CALL(glNamedBufferData(transform_buffer_handle, 12 * sizeof(float), 0, GL_STATIC_DRAW));

    float theta = 1.0 * glm::pi<float>() / 6.0f;
    float cs = std::cos(theta);
    float ss = std::sin(theta);
    glm::mat2 rot{cs, ss, -ss, cs};
    glm::vec2 trans{0.0, -0.25};
    glm::vec2 scale{0.5, 0.5};

    float transform_data[] = {
        scale.x, scale.y, trans.x, trans.y,
        rot[0].x, rot[0].y, 0.0f, 0.0f,
        rot[1].x, rot[1].y, 0.0f, 0.0f
    };

    OGL_CALL(glNamedBufferSubData(transform_buffer_handle, 0, sizeof(transform_data), transform_data));

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, transform_buffer_handle));

    ////////////////////////////////////////
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));

    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}

void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, 0));
    OGL_CALL(glBindVertexArray(0));
}
