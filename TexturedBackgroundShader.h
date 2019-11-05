//
// Created by clock on 11/27/16.
//

#ifndef PROJECT2_CMAKE_TEXTUREDBACKGROUNDSHADER_H
#define PROJECT2_CMAKE_TEXTUREDBACKGROUNDSHADER_H

#include "shader.h"
#include "film.h"
#include <string>
using std::string;
class TexturedBackgroundShader: public Shader{
public:
    unsigned int height, width;
    Pixel *data;
    TexturedBackgroundShader(Render_World &world_input, string filename);

    vec3 Shade_Surface(const Ray &ray, const vec3 &intersection_point, const vec3 &same_side_normal,
                       int recursion_depth) const override;
};


#endif //PROJECT2_CMAKE_TEXTUREDBACKGROUNDSHADER_H
