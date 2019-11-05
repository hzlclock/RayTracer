#include "cylinder.h"
#include "ray.h"
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
bool Cylinder::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    std::vector<Hit> subhits;
    bool result = thisObj->Intersection(ray, subhits);
    for (auto &&item : subhits) {
        item.object = this;
        hits.push_back(item);
    }
    return result;
}

vec3 Cylinder::Normal(const vec3 &point, int part) const {
    return thisObj->Normal(point, part);
}

bool Cylinder::isInside(const vec3 &point) {
    return thisObj->isInside(point);
}
