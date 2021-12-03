#ifndef COMP3421_TUTORIAL_05_ANIMATOR_HPP
#define COMP3421_TUTORIAL_05_ANIMATOR_HPP

#include <glm/ext.hpp>

#include <ass3/scene.hpp>

void blinking_light(node_t &scene, camera_t &cam, int now);

void animate_heart(node_t &scene, float dt, camera_t &camera);

void update_light(node_t &scene, camera_t &camera);

#endif //COMP3421_TUTORIAL_05_ANIMATOR_HPP
