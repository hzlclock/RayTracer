#include "driver.h"

bool Driver::Render_More() {
    if (state_j >= Pixel_Height()) return true;

    for (int i = 0; i < Pixel_Width(); i++) {
        ivec2 pixel_index(i, state_j);
        world.Render_Pixel(pixel_index);
    }

    state_j++;
    return false;
}
