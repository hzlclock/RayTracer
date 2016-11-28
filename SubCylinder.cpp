#include "cylinder.h"
#include "ray.h"
#include "SubCylinder.h"
#include <algorithm>
#include <cfloat>

inline double dis_lines(const vec3 &line1_st, const vec3 &line1_dic,
                        const vec3 &line2_st, const vec3 &line2_dic) {
    double arriba = dot(line1_st - line2_st, cross(line1_dic, line2_dic));
    double abajo = cross(line1_dic, line2_dic).magnitude();
    return fabs(arriba / abajo);
}

inline double dis_point_line_sq(const vec3 &point, const vec3 &line_st, const vec3 &line_dic) {
    double result = cross(point - line_st, line_dic).magnitude_squared();
    return result;
}

inline vec3 line_plane_intersection(const vec3 &line_st, const vec3 &line_dic,
                                    const vec3 &plane_pt, const vec3 &plane_n) {
    double dis = dot(line_st - plane_pt, plane_n) / dot(plane_n, line_dic);
    vec3 result = line_st - line_dic * dis;
    return result;
}

// Determine if the ray intersects with the cylinder.
bool SubCylinder::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    double dis = dis_lines(ray.endpoint, ray.direction, A, (A - B).normalized());
    if (dis < radius) {
        vec3 v = -ray.direction;
        vec3 y = ray.endpoint - B;
        vec3 vbar = cross(v, (A - B).normalized());
        vec3 ybar = cross(y, (A - B).normalized());

        Hit nuevo;
        nuevo.object = this;
        nuevo.part = 0;
        double dis_to_center = sqrt(dis_point_line_sq(ray.endpoint, A, (A - B).normalized()) - dis * dis);
        double dis_inside = sqrt(radius * radius - dis * dis);

        double arriba_p1 = -dot(vbar, ybar);
        double arriba_p2 = sqrt(dot(vbar, vbar) * (radius * radius) - cross(vbar, ybar).magnitude_squared());
        double abajo = dot(vbar, vbar);
        nuevo.t = (arriba_p1 - arriba_p2) / abajo;
        //std::cout<<ray.endpoint-ray.direction*nuevo.t<<std::endl;
        hits.push_back(nuevo);
        return true;
    }
    return false;
}

vec3 SubCylinder::Normal(const vec3 &point, int part) const {
    vec3 normal;
    // TODO: set the normal
    vec3 pt = line_plane_intersection(A, (A - B).normalized(), point, (A - B).normalized());
    normal = (point - pt).normalized();
    return normal;
}

bool SubCylinder::isInside(const vec3 &point) {
    if (dis_point_line_sq(point, A, (A - B).normalized()) < radius * radius) {
        return true;
    }
    return false;
}
