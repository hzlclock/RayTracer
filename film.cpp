#include "film.h"

Film::~Film() {
    delete[] colors;
}

void Film::Set_Resolution(const int pixels_width, const int pixels_height) {
    if (colors) delete[] colors;
    colors = new Pixel[pixels_width * pixels_height];
    pixel_grid.Initialize(ivec2(pixels_width, pixels_height), -0.5 * vec2(width, height), 0.5 * vec2(width, height));
}
