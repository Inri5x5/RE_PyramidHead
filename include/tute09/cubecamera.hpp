#ifndef COMP3421_TUTORIAL_05_CUBEMAPCAMERA_HPP
#define COMP3421_TUTORIAL_05_CUBEMAPCAMERA_HPP


#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

/**
 * A classic Euler-angle camera
 * Quite unremarkable
 */
struct cubecamera_t {
    glm::vec3 pos;
    glm::vec3 initial_pos;
    glm::vec3 initial_target;
    int mode = 0;
    float yaw;
    float pitch;
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 pv_matrix;
};

/**
 * Create a new camera at pos looking toward target
 *
 * @param pos The position of the camera
 * @param target What the camera is looking at
 * @return A new camera positioned at position, looking at target.
 */
cubecamera_t make_cubecamera(glm::vec3 pos, glm::vec3 target);



/**
 * Get the view matrix of this camera.
 *
 * The view matrix is an inverse transform that takes points in world space and moves them into the camera's space.
 * Effectively, moves the world "into" the camera's eye.
 *
 * @param cam The camera whose view we're interested in.
 * @return A 4x4 matrix that moves world-space coordinates into camera-space
 */
glm::mat4 get_view(const cubecamera_t &cam);

void switchToFace(cubecamera_t &ccamera, int faceIndex);

/**
 * Destroys the camera "c".
 *
 * Does nothing since camera_t is a trivial type and holds no resources
 *
 * @param c The camera to destroy.
 */
void delete_cubemcamera(cubecamera_t &c);

#endif // COMP3421_TUTORIAL_05_CUBEMAPCAMERA_HPP