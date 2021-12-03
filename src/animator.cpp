#include <ass3/animator.hpp>
#include <stack>
#include <cmath>

const float PERIOD = 6.f;

void blinking_light(node_t &scene, camera_t &cam, int now) {
    auto cycle = std::fmod(now, PERIOD);
    if (cycle == 3 or cycle == 5) {
        scene.point_lightB.color = glm::vec3{ 0, 0, 0 };
    } else {
        scene.point_lightB.color = glm::vec3{ 1, 1, 1 };
    }
}

void update_light(node_t &scene, camera_t &cam) {
    glm::vec3 &posA = scene.point_lightA.pos;
    posA = cam.pos;
}

void animate_heart(node_t &scene, float dt, camera_t &camera) {
    std::stack<node_t *> animatables{{ &scene }};
    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();
        switch (n->anime) {
            case node_t::ROTATION:
                n->rotation.y += glm::radians(200.0f) * dt;
                n->transform = glm::translate(glm::mat4(1.0), glm::vec3(camera.pos.x, camera.pos.y - 1, camera.pos.z ));
                n->transform *= glm::rotate(glm::mat4(1.0), n->rotation.y, glm::vec3(0, 1, 0));
                break;
            default:
                break;
        }
        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}
