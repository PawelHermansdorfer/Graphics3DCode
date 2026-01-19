#include "texture.h"

namespace xe {
    GLuint create_texture(const std::string &name, bool is_sRGB)
    {
        GLuint result;
        stbi_set_flip_vertically_on_load(true);
        GLint width, height, channels;
        // auto texture_file = std::string(ROOT_DIR) + "/" + name;
        auto texture_file = name;
        auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
        if (!img) {
            std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
        } else {
            std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
        }

        GLenum format = is_sRGB ? GL_SRGB : GL_RGB;

        glGenTextures(1, &result);
        glBindTexture(GL_TEXTURE_2D, result);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

        stbi_image_free(img);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        return(result);
    }
}
