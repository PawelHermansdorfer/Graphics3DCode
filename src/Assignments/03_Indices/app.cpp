//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

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
