#ifndef __FILM_H__
#define __FILM_H__

#include "grid.h"
#include <algorithm>

typedef unsigned int Pixel;

inline Pixel Pixel_Color(const vec3 &color) {
    unsigned int r = std::min(color[0], 1.0) * 255;
    unsigned int g = std::min(color[1], 1.0) * 255;
    unsigned int b = std::min(color[2], 1.0) * 255;
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

inline vec3 From_Pixel(Pixel color) {
    return vec3(color >> 24, (color >> 16) & 0xff, (color >> 8) & 0xff) / 255.;
}

class Film {
public:
    Pixel *colors;
    double width, height;
    Grid pixel_grid;

    Film()
            : colors(0), width(0), height(0) {
    }

    ~Film();

    void Set_Resolution(const int pixels_width, const int pixels_height);

    void Set_Resolution_pourAA(const int pixels_width, const int pixels_height);
    void Set_Pixel(const ivec2 &pixel_index, const Pixel &color) {
        int i = pixel_index[0];
        int j = pixel_index[1];
        colors[j * pixel_grid.size[0] + i] = color;
    }

    void FinishAA();

};

#endif
