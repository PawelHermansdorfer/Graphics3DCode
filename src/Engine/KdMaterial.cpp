#include "KdMaterial.h"
#include "Application/utils.h"
#include "mesh_loader.h"
#include "utils.h"
#include "texture.h"

namespace xe {
    GLuint KdMaterial::map_Kd_location_ = 0;

    KdMaterial::KdMaterial(const glm::vec4 &Kd ):                         Kd_(Kd), use_vertex_colors_(0),      texture_(0)       {}
    KdMaterial::KdMaterial(const glm::vec4 &Kd, int colors):              Kd_(Kd), use_vertex_colors_(colors), texture_(0)       {}
    KdMaterial::KdMaterial(const glm::vec4 &Kd, int colors, int texture): Kd_(Kd), use_vertex_colors_(colors), texture_(texture) {}

    void KdMaterial::init() {
        create_material_uniform_buffer(2*sizeof(glm::vec4));
        create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"}, {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});

        map_Kd_location_ = glGetUniformLocation(program(),"map_Kd");
        if(map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
        }
        xe::add_mat_function("KdMaterial", KdMaterial::create_from_mtl);
    }

    void KdMaterial::bind() const {
        glUseProgram(program());

        glUniform1i(map_Kd_location_, 0);
        if(texture_ > 0)
        {
            use_map_Kd_ = 1;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_);
        }
        else if(texture_ == 0)
        {
            use_map_Kd_ = 0;
        }

        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0,                             sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4),             sizeof(int),       &use_vertex_colors_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4)+sizeof(int), sizeof(int),       &use_map_Kd_));
    }

    void KdMaterial::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
        if (texture_ > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    Material *KdMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
        glm::vec4 color = get_color(mat.diffuse);
        SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
        auto material = new xe::KdMaterial(color);
        if (!mat.diffuse_texname.empty()) {
            auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
            SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
            if (texture > 0) {
                // material->set_texture(texture);
                material->texture_ = texture;
            }
        }
        return material;
    }
}
