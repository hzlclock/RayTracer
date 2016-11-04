#include "sphere.h"
#include "ray.h"

inline double dis_point_to_line(const vec3 &x0,
                                const vec3 &x1,
                                const vec3 &x2) {
    double up = (cross((x0 - x1), (x0 - x2))).magnitude();
    double down = (x2 - x1).magnitude();
    return up / down;
}

bool Sphere::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    bool isInside = ((ray.endpoint - center).magnitude() < radius);
    if (dot(ray.direction, (ray.endpoint - center)) < 0 && !isInside) {
        return false;
    }
    double dis = dis_point_to_line(center, ray.endpoint, ray.endpoint + ray.direction);
    //printf("%f|", dis);
    if (dis < radius) {
        Hit newhit;
        newhit.object = this;
        newhit.part = 0;
        newhit.t = sqrt((ray.endpoint - center).magnitude_squared() - dis * dis);
        newhit.t += sqrt(radius * radius - dis * dis);
        newhit.ray_exiting = false;
        hits.push_back(newhit);
        if (!isInside) {
            newhit.part = 1;
            newhit.ray_exiting = true;
            newhit.t -= 2 * sqrt(radius * radius - dis * dis);
            hits.push_back(newhit);
        }
        hits.push_back(newhit);
        return true;
    }
    return false;
}

vec3 Sphere::Normal(const vec3 &point, int part) const {
    vec3 normal;
    // TODO: set the normal
    normal = point - center;
    normal = normal.normalized();
    if (part == 0)normal = -normal;
    return normal;
}
