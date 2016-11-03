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
    float dis = dis_point_to_line(center, ray.endpoint, ray.endpoint + ray.direction);
    //printf("%f|", dis);
    if (dis < radius) {
        Hit newhit;
        newhit.object = this;
        newhit.part = ((ray.endpoint - center).magnitude() < radius);
        newhit.t = sqrt((ray.endpoint - center).magnitude_squared() - dis * dis) -
                   sqrt(radius * radius - dis * dis);
        newhit.ray_exiting = false;

        //printf("%f\n",(ray.endpoint - ray.direction*newhit.t - center).magnitude() - radius);

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
    if (part == 1)normal = -normal;
    return normal;
}
