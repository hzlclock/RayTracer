#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "object.h"
#include "vec.h"

// Points A and B are located along the axis of symmetry on the circular end
// caps. Note that the distance between A and B gives you the height of the
// cylinder.
class Cylinder : public Object {
    vec3 A, B;
    double radius;

public:
    Cylinder(const vec3 &center0, const vec3 &center1, double radius_input)
            : A(center0), B(center1), radius(radius_input) {}

    virtual bool Intersection(const Ray &ray, std::vector<Hit> &hits) const override;

    virtual vec3 Normal(const vec3 &point, int part) const override;

    virtual bool isInside(const vec3 &point) override;
};

#endif
