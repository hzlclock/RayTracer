#include "cylinder.h"
#include "ray.h"
#include <algorithm>
#include <cfloat>

// Determine if the ray intersects with the cylinder.
bool Cylinder::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    return false;
}

vec3 Cylinder::Normal(const vec3 &point, int part) const {
    vec3 normal;
    // TODO: set the normal
    return normal;
}

bool Cylinder::isInside(const vec3 &point) {
    return false;
}
