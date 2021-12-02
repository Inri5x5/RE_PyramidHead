#include <tute09/scene.hpp>
#include <tute09/cubemap.hpp>
#include <tute09/model.hpp>

const char *EXTERIOR_PATH = "res/obj/exterior/exterior.obj";
const char *EXTERIOR_PORTAL_PATH = "res/obj/exterior/exterior_portal.obj";
const char *INTERIOR_PATH = "res/obj/interior/interior.obj";
const char *INTERIOR_PORTAL_PATH = "res/obj/interior/interior_portal.obj";
const char* SKYBOX_BASE_PATH = "res/skybox/sky";
const char *HEART_PATH = "res/obj/HEART/hearts.obj";
const char *ROOM_PATH = "res/obj/ROOM/bedroom1.obj";
const char *MIRROR_PATH = "res/obj/MIRROR/mirror.obj";

node_t make_scene_interior(GLuint portal_texture) {
    node_t scene;

    node_t interior;
    interior.kind = node_t::STATIC_MESH;
    interior.obj = load_obj(INTERIOR_PATH);

    node_t interior_portal;
    interior_portal.kind = node_t::PORTAL;
    interior_portal.obj = load_obj(INTERIOR_PORTAL_PATH);
    interior_portal.texture = portal_texture;

    scene.children.push_back(interior);
    scene.children.push_back(interior_portal);

    return scene;
}

node_t make_scene_exterior(GLuint portal_texture) {
    node_t scene;

    node_t exterior;
    exterior.kind = node_t::STATIC_MESH;
    exterior.obj = load_obj(EXTERIOR_PATH);

    node_t exterior_portal;
    exterior_portal.kind = node_t::PORTAL;
    exterior_portal.obj = load_obj(EXTERIOR_PORTAL_PATH);
    exterior_portal.texture = portal_texture;

    node_t sphere;
    sphere.kind = node_t::REFLECTIVE;
    sphere.obj = load_obj(HEART_PATH);
    sphere.transform =
            glm::translate(glm::mat4(1), glm::vec3(0, 5, -15));
    sphere.texture = make_cubemap(SKYBOX_BASE_PATH);

    scene.children.push_back(exterior_portal);
    scene.children.push_back(exterior);
    scene.children.push_back(sphere);

    return scene;
}

node_t make_scene(GLuint mirror_texture, GLuint cube_texture) {
    node_t scene;
    point_light_t lightA = {
            glm::vec3{ 0, 50,-30 },
            glm::vec3{ 15, 0, 0 },
			350
    };

	point_light_t lightB = {
            glm::vec3{ 0, 15, 0 },
            glm::vec3{ 1, 1, 1 },
            0
    };

    node_t mirror;
    mirror.kind = node_t::MIRROR;
    mirror.obj = load_obj(MIRROR_PATH);
    mirror.texture = mirror_texture;
    
	node_t heartA;
    heartA.kind = node_t::REFLECTIVE;
    heartA.obj = load_obj(HEART_PATH);
    //heartA.texture = make_cubemap(SKYBOX_BASE_PATH);
    heartA.texture = cube_texture;
    
    node_t heart;
    heart.kind = node_t::STATIC_MESH;
    heart.anime = node_t::ROTATION;
    heart.obj = load_obj(HEART_PATH);
    heart.obj.emissive = glm::vec3(1, 0.2, 0);

    node_t room;
    room.kind = node_t::STATIC_MESH;
    room.obj = load_obj(ROOM_PATH); 
    room.children.push_back(mirror);
    room.children.push_back(heart);
    room.children.push_back(heartA);

    scene.children.push_back(room);
    scene.point_lightA = lightA;
    scene.point_lightB = lightB;
    return scene;
}