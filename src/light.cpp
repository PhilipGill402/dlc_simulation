#include "light.h"

Light::Light(int given_x, int given_y) {
    x = given_x;
    y = given_y;

    val = false;
    pin_in.x = given_x;
    pin_in.y = given_y;
}

void Light::draw(SDL_Renderer* renderer) {
    if (val) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }


    //draws circle
    for (int w = -INPUT_RADIUS; w <= INPUT_RADIUS; w++) {
        for (int h = -INPUT_RADIUS; h <= INPUT_RADIUS; h++) {
            if (w*w + h*h <= INPUT_RADIUS*INPUT_RADIUS) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

void Light::evaluate() {
    val = pin_in.value; 
}
