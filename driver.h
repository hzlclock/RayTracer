#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "render_world.h"

class Driver {
public:
    Render_World &world;
    bool AA = false;
    int state_j; // current rendering row

    Driver(Render_World &world_input)
            : world(world_input), state_j(0) {}

    int Pixel_Width() const {
        return world.camera.film.pixel_grid.size[0];
    }

    int Pixel_Height() const {
        return world.camera.film.pixel_grid.size[1];
    }

    bool Render_More();
};

#endif
