//
// Created by clock on 11/27/16.
//

#include "TexturedBackgroundShader.h"
#include "ray.h"
#include <png.h>

inline vec3 color_mix(const vec3 &c1, const vec3 &c2) {
    return vec3(c1.x[0] * c2.x[0], c1.x[1] * c2.x[1], c1.x[2] * c2.x[2]);
}

void Read_png_bg(Pixel *&data, unsigned int &width, unsigned int &height, const char *filename) {
    FILE *file = fopen(filename, "rb");
    assert(file);

    unsigned char header[8];
    int num_read = fread(&header, 1, sizeof header, file);
    assert(num_read == sizeof header);
    int ret_sig = png_sig_cmp((png_bytep) header, 0, sizeof header);
    assert(!ret_sig);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    assert(png_ptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr);
    png_infop end_info = png_create_info_struct(png_ptr);
    assert(end_info);
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sizeof header);
    png_read_info(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (bit_depth < 8)
        png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_swap_alpha(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_bgr(png_ptr);

    if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

    height = png_get_image_height(png_ptr, info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);

    data = new Pixel[width * height];

    png_read_update_info(png_ptr, info_ptr);

    for (int i = 0; i < height; i++)
        png_read_row(png_ptr, (png_bytep) (data + (height - i - 1) * width), 0);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(file);
}
TexturedBackgroundShader::TexturedBackgroundShader(Render_World &world_input, std::string filename)
        : Shader(world_input) {
    Read_png_bg(data, width, height, filename.c_str());
}

vec3
TexturedBackgroundShader::Shade_Surface(const Ray &ray, const vec3 &intersection_point, const vec3 &same_side_normal,
                                        int recursion_depth) const {
    double theta = atan(ray.direction.x[2] / ray.direction.x[0]);
    if (ray.direction.x[0] < 0)theta += M_PI;
    if (ray.direction.x[0] > 0 && ray.direction.x[2] < 0)theta += 2 * M_PI;
    int y = (double) height * (ray.direction.x[1] / 2.0 + 0.5);
    int x = theta / (2 * M_PI) * width;
    int index = y * width + x;
    vec3 texture_color = From_Pixel(data[index]);
    return texture_color;
}
