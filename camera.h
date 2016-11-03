#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "film.h"

class Camera {
public:
    vec3 position; // camera position
    vec3 focal_point; // where the image plane is located
    vec3 look_vector; // points from the position to the focal point - normalized
    vec3 vertical_vector; // point up in the image plane - normalized
    vec3 horizontal_vector; // points to the right on the image plane - normalized
    Film film;

    Camera()
            : position(0, 0, -1), focal_point(0, 0, 0), vertical_vector(0, 1, 0) {}

    void Position_And_Aim_Camera(const vec3 &position_input,
                                 const vec3 &look_at_point, const vec3 &pseudo_up_vector);

    void Focus_Camera(double focal_distance, double aspect_ratio,
                      double field_of_view);

    vec3 World_Position(const ivec2 &pixel_index);
};

#endif
