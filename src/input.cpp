#include "input.h"

Input::Input(int given_x, int given_y) {
    x = given_x;
    y = given_y;
    val = false;
    pin_out.x = given_x;
    pin_out.y = given_y;
}

void Input::toggle() {
    val = !val;
}

void Input::draw(SDL_Renderer* renderer) {
    if (val) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }


    //draws circle
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius*radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

