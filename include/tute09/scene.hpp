#ifndef COMP3421_TUTORIAL_09_SCENE_HPP
#define COMP3421_TUTORIAL_09_SCENE_HPP

#include <glad/glad.h>

#include <glm/ext.hpp>

#include <tute09/camera.hpp>
#include <tute09/model.hpp>

struct point_light_t {
    glm::vec3 pos;
    glm::vec3 color;
    float attenuation;
};

struct node_t {
    /**
     * The different types of models in our scene.
     */
    enum {
        EMPTY,
        STATIC_MESH,
        REFLECTIVE,
        PORTAL,
        MIRROR,
        HEART
    } kind = EMPTY;
    glm::mat4 transform = glm::mat4(1);
    std::vector<node_t> children {};
    point_light_t point_lightA;
    point_light_t point_lightB;
    glm::vec3 translation = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0); // vec3 of euler angles
    glm::vec3 scale = glm::vec3(1.0);

    // Only for STATIC_MESH, REFLECTIVE, PORTAL:
    model_t obj;

    // Only for REFLECTIVE, PORTAL:
    GLuint texture;

    // Only for PORTAL:
    bool is_interior;
};


/**
 * Create the interior scene
 */
node_t make_scene_interior(GLuint portal_texture);

/**
 * Create the exterior scene
 */
node_t make_scene_exterior(GLuint portal_texture);

node_t make_scene(GLuint mirror_texture);

#endif //COMP3421_TUTORIAL_09_SCENE_HPP
