#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "object.h"
#include "vec.h"
#include "SubCylinder.h"
#include "boolean.h"
#include "plane.h"

// Points A and B are located along the axis of symmetry on the circular end
// caps. Note that the distance between A and B gives you the height of the
// cylinder.
class Cylinder : public Object {
    vec3 A, B;
    SubCylinder subCylinder;
    Plane *p1, *p2;
    Boolean *thisObj;

public:
    Cylinder(const vec3 &center0, const vec3 &center1, double radius_input)
            : subCylinder(center0, center1, radius_input), A(center0), B(center1) {
        p1 = new Plane(center0, center1 - center0);
        p2 = new Plane(center1, center1 - center0);
        Boolean *region_planes = new Boolean(p1, p2, Boolean::type_intersection);
        thisObj = new Boolean(region_planes, &subCylinder, Boolean::type_intersection);
    }

    virtual bool Intersection(const Ray &ray, std::vector<Hit> &hits) const override;

    virtual vec3 Normal(const vec3 &point, int part) const override;

    virtual bool isInside(const vec3 &point) override;
};

#endif
