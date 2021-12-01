#include <glad/glad.h>

#include <chicken3421/chicken3421.hpp>

#include <tute09/cubemap.hpp>

namespace {
    const char* side_suffixes[] = {
            "_right", "_left", "_top", "_bottom", "_front", "_back"
    };

    void load_side(int side, const std::string &path) {
        chicken3421::image_t image = chicken3421::load_image(path, false);
        GLenum fmt = image.n_channels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                     0, fmt, image.width, image.height, 0, fmt, GL_UNSIGNED_BYTE, image.data);
        chicken3421::delete_image(image);
    }
}

GLuint make_cubemap(
        const std::string &base_path,
        const std::string &extension) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++) {
        load_side(i, base_path + side_suffixes[i] + extension);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture;
}
