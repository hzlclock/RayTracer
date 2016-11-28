#include "film.h"

Film::~Film() {
    delete[] colors;
}

void Film::Set_Resolution(const int pixels_width, const int pixels_height) {
    if (colors) delete[] colors;
    colors = new Pixel[pixels_width * pixels_height];
    pixel_grid.Initialize(ivec2(pixels_width, pixels_height), -0.5 * vec2(width, height), 0.5 * vec2(width, height));
}

void Film::FinishAA() {
    Pixel *finalFilmColors = new Pixel[(pixel_grid.size.x[0] *
                                        pixel_grid.size.x[1]) / 4];

    for (int i = 0; i < pixel_grid.size.x[0] / 2; ++i) {
        for (int j = 0; j < pixel_grid.size.x[1] / 2; ++j) {
            vec3 unColor[4] =
                    {From_Pixel(colors[i + i + (j * 2) * (pixel_grid.size.x[0])]),
                     From_Pixel(colors[i + i + 1 + (j * 2) * (pixel_grid.size.x[0])]),
                     From_Pixel(colors[i + i + (j * 2 + 1) * (pixel_grid.size.x[0])]),
                     From_Pixel(colors[i + i + 1 + (j * 2 + 1) * (pixel_grid.size.x[0])])};
            vec3 finalColor(0, 0, 0);
            for (auto &&item : unColor) {
                finalColor += item / 4;
            }
            finalFilmColors[i + j * pixel_grid.size.x[0] / 2] = Pixel_Color(finalColor);
        }
    }
    delete colors;
    colors = finalFilmColors;
    pixel_grid.size.x[0] /= 2;
    pixel_grid.size.x[1] /= 2;
}

void Film::Set_Resolution_pourAA(const int pixels_width, const int pixels_height) {
    if (colors) delete[] colors;
    colors = new Pixel[pixels_width * pixels_height * 4];
    pixel_grid.Initialize(ivec2(pixels_width * 2, pixels_height * 2), -0.5 * vec2(width, height),
                          0.5 * vec2(width, height));
}
