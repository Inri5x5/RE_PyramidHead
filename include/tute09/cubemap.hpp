#ifndef COMP3421_TUTORIAL_09_CUBEMAP_HPP
#define COMP3421_TUTORIAL_09_CUBEMAP_HPP

#include <glad/glad.h>
#include <string>

/**
 * Create a cubemap from 6 textures
 * @param base_path Path of cubemap textures, without extension
 * @param extension File extension for cubemap textures, with dot
 * @return OpenGL texture handle
 */
GLuint make_cubemap(const std::string &base_path, const std::string &extension = ".jpg");

#endif //COMP3421_TUTORIAL_09_CUBEMAP_HPP
