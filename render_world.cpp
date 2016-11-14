#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

Render_World::Render_World()
        : background_shader(0), ambient_intensity(0), enable_shadows(true),
          recursion_depth_limit(3) {}

Render_World::~Render_World() {
    delete background_shader;
    for (size_t i = 0; i < objects.size(); i++) delete objects[i];
    for (size_t i = 0; i < lights.size(); i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object *Render_World::Closest_Intersection(Ray &ray, Hit &hit) {
    // TODO
    std::vector<Hit> hits;
    for (auto &entry: objects) {
        bool hitted = entry->Intersection(ray, hits);
    }
    double closet_hit_value = std::numeric_limits<double>::max();
    if (hits.size() == 0)
        return 0;
    else if (hits.size() == 1) {
        hit = hits[0];
        return (Object *) hits[0].object;
    }
    if (hits.size() > 1) {
        for (auto &entry: hits) {
            if (closet_hit_value > entry.t) {
                closet_hit_value = entry.t;
                hit = entry;
            }
        }
        return (Object *) hit.object;
    }

    return 0;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2 &pixel_index) {
    Ray ray;

    // TODO: set up the initial view ray here
    // call camera's worldposition to get a vector towards the object and cast that ray
    vec3 look_direction = camera.World_Position(pixel_index);
    ray = Ray(camera.position, look_direction);

    // then get the ray's color using Cast_Ray
    vec3 color = Cast_Ray(ray, 1);
    camera.film.Set_Pixel(pixel_index, Pixel_Color(color));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(Ray &ray, int recursion_depth) {
    // TODO
    static vec3 null_hitpoint;
    vec3 color;
    // determine the color here
    // First call intersection to determine if the ray hits something
    Hit hit;
    Object *closest_object = Closest_Intersection(ray, hit);

    // Then, do shading!
    if (closest_object) {
        vec3 hitpoint = ray.endpoint - hit.t * ray.direction;
        closest_object->material_shader->dueno = closest_object;
        color = closest_object->
                material_shader->
                Shade_Surface(ray, hitpoint,
                              closest_object->Normal(hitpoint, hit.part),
                              recursion_depth);

    } else {
        color = background_shader->Shade_Surface(ray, null_hitpoint, null_hitpoint, 1);
    }
    return color;
}
