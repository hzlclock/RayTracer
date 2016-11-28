#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 reflect2(const vec3 &v, const vec3 &normal) {
    return -2 * dot(v, normal) * normal + v;
}
vec3 Reflective_Shader::
Shade_Surface(const Ray &ray, const vec3 &intersection_point,
              const vec3 &same_side_normal, int recursion_depth) const {
    vec3 color;
    // TODO: determine the color
    shader->dueno = this->dueno;
    if (recursion_depth <= 1) {
        color = shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth);
    } else {
        Ray reflectedRay(intersection_point, reflect2(ray.direction, same_side_normal));
        color = reflectivity * world.Cast_Ray(reflectedRay, recursion_depth - 1) +
                (1 - reflectivity) * shader->Shade_Surface(ray, intersection_point, same_side_normal, recursion_depth);
    }
    return color;
}
