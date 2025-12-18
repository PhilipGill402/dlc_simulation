#include "input.h"

Input::Input(int given_x, int given_y) {
    id = 0; 
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
    for (int w = -LIGHT_RADIUS; w <= LIGHT_RADIUS; w++) {
        for (int h = -LIGHT_RADIUS; h <= LIGHT_RADIUS; h++) {
            if (w*w + h*h <= LIGHT_RADIUS*LIGHT_RADIUS) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

