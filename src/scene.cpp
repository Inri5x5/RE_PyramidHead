#include <ass3/scene.hpp>
#include <ass3/cubemap.hpp>
#include <ass3/model.hpp>

const char *HEART_PATH = "res/obj/HEART/hearts.obj";
const char *ROOM_PATH = "res/obj/SCENE1/bedroom1.obj";
const char *ROOM2_PATH = "res/obj/SCENE2/untitled.obj";
const char *MIRROR_PATH = "res/obj/MIRROR/mirror.obj";
const char *BOTTLE_PATH = "res/obj/BOTTLE/bottle.obj";


node_t make_scene(GLuint mirror_texture, GLuint cube_texture) {
    node_t scene;
    point_light_t lightA = {
            glm::vec3{ 0, 50,-30 },
            glm::vec3{ 20, 0, 0 },
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
    
	node_t reflect;
    reflect.kind = node_t::REFLECTIVE;
    reflect.obj = load_obj(HEART_PATH);
    reflect.texture = cube_texture;
    reflect.transform = glm::translate(glm::mat4(1), glm::vec3(0, 5, 0));
    
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
    room.children.push_back(reflect);

    scene.children.push_back(room);
    scene.point_lightA = lightA;
    scene.point_lightB = lightB;
    return scene;
}

node_t make_scene2(GLuint mirror_texture) {
    node_t scene;
    point_light_t lightA = {
            glm::vec3{ 0, 50,-30 },
            glm::vec3{ 0, 0, 20 },
			350
    };

	point_light_t lightB = {
            glm::vec3{ 0, 14, 0 },
            glm::vec3{ 1, 1, 1 },
            0
    };

    node_t mirror;
    mirror.kind = node_t::MIRROR;
    mirror.obj = load_obj(MIRROR_PATH);
    mirror.texture = mirror_texture;
    
    node_t heart;
    heart.kind = node_t::STATIC_MESH;
    heart.anime = node_t::ROTATION;
    heart.obj = load_obj(HEART_PATH);
    heart.obj.emissive = glm::vec3(0.3, 0, 1);

    node_t room;
    room.kind = node_t::STATIC_MESH;
    room.obj = load_obj(ROOM2_PATH);
    room.children.push_back(mirror);
    room.children.push_back(heart);

    scene.children.push_back(room);
    scene.point_lightA = lightA;
    scene.point_lightB = lightB;
    return scene;
}