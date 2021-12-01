#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <chicken3421/chicken3421.hpp>

#include <tute09/camera.hpp>
#include <tute09/memes.hpp>
#include <tute09/renderer.hpp>
#include <tute09/scene.hpp>
#include <tute09/framebuffer.hpp>
#include <tute09/animator.hpp>

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

int main() {
    chicken3421::enable_debug_output();
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1280, 720, "Tutorial 09"));
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera_t cam = make_camera({ 0, 10, 0 }, {-10, 10, 0 });
    int fb_width, fb_height;
    glfwGetFramebufferSize(win, &fb_width, &fb_height);
    framebuffer_t framebuffer = make_framebuffer(fb_width, fb_height);
    framebuffer_t HDRframebuffer = make_HDRframebuffer(fb_width, fb_height);
    node_t scene1 = make_scene(framebuffer.texture);
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.001, 1000.0));

    while (!glfwWindowShouldClose(win)) {
        float dt = time_delta();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // teleport camera between interior/exterior scene
        // // when it goes through a portal
        // glm::vec3 f = cam.pos + glm::vec3(get_view(cam) * glm::vec4(0,0,-0.2,0));
        // if (f.x > -1.3f && f.x < 1.3f &&
        //     f.y > 2.5f && f.y < 7.5f &&
        //     f.z < -9.5f && f.z > -10.5f) {
        //     if (camera_in_interior && f.z < -10) {
        //         camera_in_interior = false;
        //     } else if (!camera_in_interior && f.z > -10) {
        //         camera_in_interior = true;
        //     }
        // }

        update_camera(cam, win, dt);
        update_light(scene1, cam);
        animate_heart(scene1, dt, cam);

        //calculate clipping plane equation in view space
        // glm::mat4 view = get_view(cam);
        // glm::vec3 plane_point(0, 0, -10);
        // glm::vec3 plane_normal(0, 0, camera_in_interior ? -1 : 1);
        // glm::vec3 view_ppoint = glm::vec3(view * glm::vec4(plane_point, 1));
        // glm::vec3 view_pnormal = glm::mat3(view) * plane_normal;
        // float view_pd = glm::dot(-view_ppoint, view_pnormal);
        // renderer.portal_clip_plane = glm::vec4(view_pnormal, view_pd);


        camera_t cam2 = cam;
        float x_distance = cam.pos.x - (-15.8);
        cam2.pos.x -= x_distance * 2;
        cam2.yaw *= -1;
        //render to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_CLIP_DISTANCE0);
        render(renderer, cam2, scene1);
        //glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        //render to HDR framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, HDRframebuffer.fbo);
        render(renderer, cam, scene1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // render to screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad(renderer, HDRframebuffer.texture);

        // // render normal scene
        // if (camera_in_interior) {
        //     render(renderer, cam, scene_interior);
        // } else {
        //     render(renderer, cam, scene_exterior);
        // }
        glfwSwapBuffers(win);
        glfwPollEvents();

        // not entirely correct as a frame limiter, but close enough
        // it would be more correct if we knew how much time this frame took to render
        // and calculated the distance to the next "ideal" time to render and only slept that long
        // the current way just always sleeps for 16.67ms, so in theory we'd drop frames
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }

    // deleting the whole window also removes the opengl context, freeing all our memory in one fell swoop.
    chicken3421::delete_opengl_window(win);

    return EXIT_SUCCESS;
}


float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float)glfwGetTime();
}