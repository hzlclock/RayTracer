//
// Created by clock on 11/27/16.
//

#ifndef PROJECT2_CMAKE_SUBCYLINDER_H
#define PROJECT2_CMAKE_SUBCYLINDER_H

#include "object.h"
#include "vec.h"

class SubCylinder : public Object {
    vec3 A, B;
    double radius;

public:
    SubCylinder(const vec3 &center0, const vec3 &center1, double radius_input)
            : A(center0), B(center1), radius(radius_input) {}

    virtual bool Intersection(const Ray &ray, std::vector<Hit> &hits) const override;

    virtual vec3 Normal(const vec3 &point, int part) const override;

    virtual bool isInside(const vec3 &point) override;
};

#endif //PROJECT2_CMAKE_SUBCYLINDER_H
