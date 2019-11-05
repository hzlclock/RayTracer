#ifndef __RENDER_WORLD_H__
#define __RENDER_WORLD_H__

#include <vector>
#include "camera.h"
#include "hit.h"

class Object;

class Light;

class Shader;

class Ray;

class Render_World {
public:
    Shader *background_shader;
    std::vector<Object *> objects;
    std::vector<Light *> lights;
    vec3 ambient_color; //环境光颜色
    double ambient_intensity;
    Camera camera;
    bool enable_shadows, antialias;
    int recursion_depth_limit;

    Render_World();

    ~Render_World();

    void Render_Pixel(const ivec2 &pixel_index);

    vec3 Cast_Ray(Ray &ray, int recursion_depth);

    Object *Closest_Intersection(Ray &ray, Hit &hit);
};

#endif
