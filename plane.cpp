#include "plane.h"
#include "ray.h"
#include <cfloat>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    vec3 pos_x1 = x1 - ray.endpoint;
    double ray_normal = dot(ray.direction, normal);
    double dis_ray_to_plane = dot(pos_x1, normal) / ray_normal;
    if (dis_ray_to_plane < 0) {
        Hit newhit;
        newhit.object = this;
        newhit.part = (ray_normal > 0);
        newhit.t = dis_ray_to_plane;
        if (newhit.t < 0) newhit.t = -newhit.t;
        //std::cout<<ray.endpoint - ray.direction*newhit.t<<std::endl;
        newhit.ray_exiting = (ray_normal > 0);
        hits.push_back(newhit);
        return true;
    }
    return false;
}

vec3 Plane::
Normal(const vec3 &point, int part) const {
    if (part == 0) return -normal;
    return normal;
}

bool Plane::isInside(const vec3 &point) {
    bool result = (bool) (dot(point - x1, normal) < 0);
    return result;
}
