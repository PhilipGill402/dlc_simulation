#pragma once
#include <SDL.h>
#include "gate.h"

constexpr int radius = 10;

class Input {
public:
    int x;
    int y;
    bool val;
    Pin pin_out;

    Input(int given_x, int given_y);
    void toggle();
    void draw(SDL_Renderer* renderer);
};
