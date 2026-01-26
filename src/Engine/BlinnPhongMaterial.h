#pragma once

#include <glm/vec4.hpp>
#include "AbstractMaterial.h"
#include "ObjectReader/sMesh.h"

namespace xe {
    class BlinnPhongMaterial : public AbstractMaterial<BlinnPhongMaterial> {
        public:
            BlinnPhongMaterial(const glm::vec4 &Kd);
            BlinnPhongMaterial(const glm::vec4 &Kd , int use_colors);
            BlinnPhongMaterial(const glm::vec4 &Kd, int colors, int map);

            static void init();

            void bind() const override;

            void unbind() const override;

            static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);

        private:
            static GLuint map_Kd_location_;
            GLuint texture_;
            mutable int use_map_Kd_;

            glm::vec4 Kd_;
            glm::vec4 Ka_;
            int use_vertex_colors_;

            glm::vec4 Ks_;
            float Ns_;
    };
}
