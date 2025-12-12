#pragma once

#include <glm/vec4.hpp>
#include "AbstractMaterial.h"

namespace xe {
    class KdMaterial : public AbstractMaterial<KdMaterial> {
        public:
            KdMaterial(const glm::vec4 &Kd);
            KdMaterial(const glm::vec4 &Kd , int use_colors);
            KdMaterial::KdMaterial(const glm::vec4 &Kd, int colors, int map);

            static void init();

            void bind() const override;

            void unbind() const override;

        private:
            static GLuint map_Kd_location_;
            GLuint texture_;
            mutable int use_map_Kd_;

            glm::vec4 Kd_;
            int use_vertex_colors_;
    };
}
