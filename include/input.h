#pragma once
#include <SDL.h>
#include "gate.h"
#include "constants.h"

class Input {
public:
    uint32_t id; 
    int x;
    int y;
    bool val;
    Pin pin_out;

    Input(int given_x, int given_y);
    void toggle();
    void draw(SDL_Renderer* renderer);
};
