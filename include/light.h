#pragma once
#include <SDL.h>
#include "gate.h"
#include "constants.h"

class Light {
public:
    uint32_t id; 
    int x;
    int y;
    bool val;
    Pin pin_in;

    Light(int given_x, int given_y);
    void draw(SDL_Renderer* renderer);
    void evaluate();
};
