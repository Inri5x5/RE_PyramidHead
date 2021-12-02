#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#include <tute09/cubecamera.hpp>

namespace {
    const float INVERT_X = -1.f;
    const float INVERT_Y = 1.f;
    const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 200.0f;
	const float FOV = 90;// don't change!
	const float ASPECT_RATIO = 1;

    glm::vec3 sidev(float p, float y) {
        auto cy = std::cos(y);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cy, 0, -sy });
    }

    glm::vec3 upv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ -sp * sy, cp, -cy * sp });
    }

    glm::vec3 frontv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cp * sy, sp, cy * cp });
    }
}

cubecamera_t make_cubecamera(glm::vec3 pos, glm::vec3 target) {
    glm::vec3 front = glm::normalize(target - pos);
    float yaw = -std::atan2(front.x, -front.z);
    float pitch = -std::asin(front.y);
    float pi = 22/7;
    float y_scale = (float) ((1.0f / tan((FOV / 2.0f) * (pi/180))));
    float x_scale = y_scale / ASPECT_RATIO;
    float frustum_length = FAR_PLANE - NEAR_PLANE;
    float matrix[16] = {0};

    matrix[0] = x_scale;
    // projection.m00 = x_scale;
    matrix[5] = y_scale;
    // projection.m11 = y_scale;
    matrix[10] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
    // projectionMatrix.m22 = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
    matrix[14] = -1;
    // projectionMatrix.m23 = -1;
    matrix[11] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
    // projectionMatrix.m32 = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
    matrix[15] = 0;
    // projectionMatrix.m33 = 0;

    return {pos, pos, target, 0, yaw, pitch, glm::make_mat4(matrix)};
}

void switchToFace(cubecamera_t &ccamera, int faceIndex) {
    switch (faceIndex) {
        case 0:
            ccamera.pitch = 0;
            ccamera.yaw = -90;
            break;
        case 1:
            ccamera.pitch = 0;
            ccamera.yaw = 90;
            break;
        case 2:
            ccamera.pitch = -90;
            ccamera.yaw = 180;
            break;
        case 3:
            ccamera.pitch = 90;
            ccamera.yaw = 180;
            break;
        case 4:
            ccamera.pitch = 0;
            ccamera.yaw = 180;
            break;
        case 5:
            ccamera.pitch = 0;
            ccamera.yaw = 0;
            break;
    }
}



glm::mat4 get_view(const cubecamera_t &cam) {
    auto s = sidev(cam.pitch, cam.yaw);
    auto u = upv(cam.pitch, cam.yaw);
    auto f = frontv(cam.pitch, cam.yaw);
    auto o = glm::vec3(-glm::dot(cam.pos, s), -glm::dot(cam.pos, u), -glm::dot(cam.pos, f));
    auto view = glm::mat4{
            s.x, u.x, f.x, 0,
            s.y, u.y, f.y, 0,
            s.z, u.z, f.z, 0,
            o.x, o.y, o.z, 1
    };

    return view;
}

void delete_cubecamera(cubecamera_t &c) {
    // nothing to do as this is a trivially destructible type
    (void)c;
}