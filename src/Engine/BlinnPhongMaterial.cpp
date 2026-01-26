#include "BlinnPhongMaterial.h"
#include "Application/utils.h"
#include "mesh_loader.h"
#include "utils.h"
#include "texture.h"

namespace xe {
    GLuint BlinnPhongMaterial::map_Kd_location_ = 0;

    BlinnPhongMaterial::BlinnPhongMaterial (const glm::vec4 &Kd ):                         Kd_(Kd), use_vertex_colors_(0),      texture_(0)       {}
    BlinnPhongMaterial::BlinnPhongMaterial (const glm::vec4 &Kd, int colors):              Kd_(Kd), use_vertex_colors_(colors), texture_(0)       {}
    BlinnPhongMaterial::BlinnPhongMaterial (const glm::vec4 &Kd, int colors, int texture): Kd_(Kd), use_vertex_colors_(colors), texture_(texture) {}

    void BlinnPhongMaterial::init() {
        create_material_uniform_buffer(3*sizeof(glm::vec4) + 2*sizeof(int) + 1*sizeof(float));
        create_program_in_engine({{GL_VERTEX_SHADER, "BlinnPhong_vs.glsl"}, {GL_FRAGMENT_SHADER, "BlinnPhong_fs.glsl"}});

        map_Kd_location_ = glGetUniformLocation(program(),"map_Kd");
        if(map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
        }
        xe::add_mat_function("BlinnPhongMaterial", BlinnPhongMaterial::create_from_mtl);
    }


    void BlinnPhongMaterial::bind() const {
        glUseProgram(program());

        glUniform1i(map_Kd_location_, 0);

        if(texture_ > 0) {
            use_map_Kd_ = 1;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_);
        } else {
            use_map_Kd_ = 0;
        }

        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Ka_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &Ks_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(float), &Ns_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float), sizeof(int), &use_vertex_colors_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float) + sizeof(int), sizeof(int), &use_map_Kd_));
    }


    void BlinnPhongMaterial ::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
        if (texture_ > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    Material *BlinnPhongMaterial ::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
        glm::vec4 color = get_color(mat.diffuse);
        SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
        auto material = new xe::BlinnPhongMaterial (color);
        if (!mat.diffuse_texname.empty()) {
            auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
            SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
            if (texture > 0) {
                // material->set_texture(texture);
                material->texture_ = texture;
            }
        }
        material->Ka_[0] = mat.ambient[0];
        material->Ka_[1] = mat.ambient[1];
        material->Ka_[2] = mat.ambient[2];

        material->Ks_ = glm::vec4(mat.specular[0], mat.specular[1], mat.specular[2], 1.0f);
        material->Ns_ = mat.shininess;

        return material;
    }
}
