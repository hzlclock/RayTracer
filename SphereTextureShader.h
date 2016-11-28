//
// Created by clock on 11/22/16.
//

#ifndef PROJECT2_CMAKE_TEXTURESHADER_H
#define PROJECT2_CMAKE_TEXTURESHADER_H

#include "shader.h"
#include <string>

using std::string;

class SphereTextureShader : public Shader {
public:
    unsigned int *data;
    unsigned int width, height, bytesPerPixel;

    SphereTextureShader(Render_World &world_input, const Shader *in_shader, string texture_filename);

    const Shader *shader;

    vec3 Shade_Surface(const Ray &ray, const vec3 &intersection_point, const vec3 &same_side_normal,
                       int recursion_depth) const override;
};


#endif //PROJECT2_CMAKE_TEXTURESHADER_H
