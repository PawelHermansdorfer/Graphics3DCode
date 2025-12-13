#pragma once

#include <string>
#include <iostream>

#include "stb/stb_image.h"
#include "glad/gl.h"

namespace xe {
  GLuint create_texture(const std::string &name, bool is_sRGB = true);
}
