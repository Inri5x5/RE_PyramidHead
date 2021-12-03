#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <chicken3421/chicken3421.hpp>

#include <ass3/camera.hpp>
#include <ass3/memes.hpp>
#include <ass3/renderer.hpp>
#include <ass3/scene.hpp>
#include <ass3/framebuffer.hpp>
#include <ass3/animator.hpp>

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

// upadate = false => SCENE1
// update = true => SCENE2
// Move the player to one room, and display the reflection like mirror of the other room
void update_room(GLFWwindow *win, int key, int, int action, int) {
    bool *update = (bool *) glfwGetWindowUserPointer(win);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        *update = !*update;
    }
}

int main() {
    chicken3421::enable_debug_output();
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1280, 720, "Assignment3"));
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera_t cam = make_camera({ 0, 10, 0 }, {-10, 10, 0 });
    int fb_width, fb_height;
    int cm_size = 128;
    glfwGetFramebufferSize(win, &fb_width, &fb_height);
    framebuffer_t framebuffer = make_framebuffer(fb_width, fb_height);
    framebuffer_t HDRframebuffer = make_HDRframebuffer(fb_width, fb_height);
    framebuffer_t scary_framebuffer = make_framebuffer(fb_width, fb_height);

    framebuffer_t CMframebuffer = make_CubeMapframebuffer(cm_size, cm_size);
    camera_t cubecamera = make_camera({ 0.0, 6.0, 0.0 }, {0, 0, 0 });

    gframebuffer_t gframebuffer = make_gframebuffer(fb_width, fb_height);

    node_t scene1 = make_scene(framebuffer.texture, CMframebuffer.texture);
    node_t scene2 = make_scene2(framebuffer.texture);
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<double>() / 3, 1280.0 / 720, 0.001, 1000.0));

    bool update = false;
    glfwSetWindowUserPointer(win, &update);
    glfwSetKeyCallback(win, update_room);

    while (!glfwWindowShouldClose(win)) {
        float dt = time_delta();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // animating and updating the movement of the ehart and its light
        update_camera(cam, win, dt);
        update_light(scene1, cam);
        animate_heart(scene1, dt, cam);
        update_light(scene2, cam);
        blinking_light(scene2, cam, time_now());
        animate_heart(scene2, dt, cam);

        node_t scene;
        if (update) {
            scene = scene2;
        } else {
            scene = scene1;
        }
        
        // rendering real-time cubemap inside scene1 to a cubemap texture
        glBindFramebuffer(GL_FRAMEBUFFER, CMframebuffer.fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, cm_size, cm_size);
		for (int i = 0; i < 6; i++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, CMframebuffer.texture, 0);
			switchToFace(cubecamera,i);
			render(renderer, cubecamera, scene);
		}
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, fb_width, fb_height);

        // rendering planar reflection (mirror) on each room inside a texture
        camera_t cam2 = cam;
        float x_distance = cam.pos.x - (-15.8);
        cam2.pos.x -= x_distance * 2;
        cam2.yaw *= -1;
        //calculate clipping plane equation in view space
        glm::mat4 view = get_view(cam2);
        glm::vec3 plane_point(0, 0, -15);
        glm::vec3 plane_normal(0, 0, 1 );
        glm::vec3 view_ppoint = glm::vec3(view * glm::vec4(plane_point, 1));
        glm::vec3 view_pnormal = glm::mat3(view) * plane_normal;
        float view_pd = glm::dot(-view_ppoint, view_pnormal);
        renderer.portal_clip_plane = glm::vec4(view_pnormal, view_pd);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CLIP_DISTANCE0);
        node_t reflected_scene;
        if (update) {
            reflected_scene = scene1;
        } else {
            reflected_scene = scene2;
        }
        render(renderer, cam2, reflected_scene);
        glDisable(GL_CLIP_DISTANCE0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Attempt on reading the geometry of every object inside gbuffer
        glBindFramebuffer(GL_FRAMEBUFFER,gframebuffer.fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        grender(renderer, cam, scene);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Rendering the final pass of gbuffer (lightning pass), but unable to render the static_mesh (mirror and reflective are fine)
        // Uncomment line 109 and comment out line 111- 119 to see
        //grender_lightning(gframebuffer.gPosition, gframebuffer.gNormal, gframebuffer.gAlbedo, scene1, cam, renderer);

        if (update) {
            // Rendering the scene -> quad framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, scary_framebuffer.fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render(renderer, cam, scene);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Adding kernerls effect to the previously render quad
            glBindFramebuffer(GL_FRAMEBUFFER, HDRframebuffer.fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderKernel(renderer, scary_framebuffer.texture);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Fianlly render the quad with HDR effect to the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderQuad(renderer, HDRframebuffer.texture);
        } else {
            // Rendering the scene -> quad framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, HDRframebuffer.fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render(renderer, cam, scene);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Fianlly render the quad with HDR effect to the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderQuad(renderer, HDRframebuffer.texture);
        }

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