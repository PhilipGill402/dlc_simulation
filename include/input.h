#pragma once
#include <SDL.h>

constexpr int radius = 10;

class Input {
public:
    int x;
    int y;
    bool val;

    Input(int given_x, int given_y);
    void toggle();
    void draw(SDL_Renderer* renderer);
};
