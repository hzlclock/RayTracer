#ifndef __GRID_H__
#define __GRID_H__

#include "vec.h"

class Grid {
public:
    ivec2 size; // # of points: x and y direction
    vec2 min, max; // location of walls: min = (left,bottom), max = (right,top)
    vec2 cell_size;

    Grid() {
    }

    Grid(const ivec2 &size_input, const vec2 &min_input, const vec2 &max_input) {
        Initialize(size_input, min_input, max_input);
    }

    void Initialize(const ivec2 &size_input, const vec2 &min_input, const vec2 &max_input) {
        size = size_input;
        min = min_input;
        max = max_input;
        cell_size = (max - min) / vec2(size);
    }

    vec2 Cell_Center(const ivec2 &index) const {
        return min + (vec2(index) + vec2(.5, .5)) * cell_size;
    }
};

#endif
