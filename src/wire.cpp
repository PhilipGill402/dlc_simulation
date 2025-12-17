#include "wire.h"

Wire::Wire(Input* input) {
    src = new Gate(input->val);
    x = input->x;
    y = input->y;
    
    first = 0; 
}

Wire::Wire(Gate* gate) {
    src = gate;
    x = gate->x;
    y = gate->y;

    first = 0; 
}

void Wire::draw(SDL_Renderer* renderer) {
    SDL_Rect vert_rect;
    SDL_Rect horizontal_rect;
    if (first == 1) {
        vert_rect = { x + width, y, 5, height };
        horizontal_rect = { x, y, width, 5};
    } else if (first == 2) {
        vert_rect = { x, y, 5, height };
        horizontal_rect = { x, y + height, width, 5};
    } else {
        //neither has been set so don't draw anyting
        return;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &vert_rect);
    SDL_RenderFillRect(renderer, &horizontal_rect);
}

void Wire::connect(Input* input, Gate* gate) {
    gate->p_inputs[gate->num_inputs] = new Gate(input->val);
    gate->num_inputs++;
}

void Wire::connect(Gate* input, Gate* gate) {
    gate->p_inputs[gate->num_inputs] = input;
    gate->num_inputs++;
}
