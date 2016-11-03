#include "camera.h"

void Camera::Position_And_Aim_Camera(const vec3 &position_input,
                                     const vec3 &look_at_point, const vec3 &pseudo_up_vector) {
    position = position_input;
    look_vector = (look_at_point - position).normalized();
    horizontal_vector = cross(look_vector, pseudo_up_vector).normalized();
    vertical_vector = cross(horizontal_vector, look_vector).normalized();
}

void Camera::Focus_Camera(double focal_distance, double aspect_ratio,
                          double field_of_view) {
    focal_point = position + look_vector * focal_distance;
    film.width = (double) 2 * focal_distance * tan((double) .5 * field_of_view);
    film.height = film.width / aspect_ratio;
}

// Find the world position of the input pixel
vec3 Camera::World_Position(const ivec2 &pixel_index) {
    vec3 result;
    static double x_scale = film.width / film.pixel_grid.size.x[0];
    static double y_scale = film.height / film.pixel_grid.size.x[1];
    // TODO

    // i would like to give a vector pointing towards that pixel

    double x_offset = (pixel_index.x[0] - (float) film.pixel_grid.size.x[0] / 2.0 + 0.5) * x_scale;
    double y_offset = (pixel_index.x[1] - (float) film.pixel_grid.size.x[1] / 2.0 + 0.5) * y_scale;

    result = horizontal_vector * x_offset +
             vertical_vector * y_offset +
             focal_point;

    return position - result;
}
