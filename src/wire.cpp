#include "wire.h"
#include <iostream>

Wire::Wire(Input* given_input) {
    input = given_input;
    src_gate = nullptr;
    dst_gate = nullptr;
    dst_idx = nullptr;
    
    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire(Gate* gate) {
    input = nullptr;
    src_gate = gate;
    dst_gate = nullptr;
    dst_idx = nullptr;
    
    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

void Wire::draw(SDL_Renderer* renderer) {
    //if dst is set then the wire is connected 
    if (dst) {
        if (first == 1) {
            vert_rect = { x + dst->x - 5, y, 5, dst->y };
            horizontal_rect = { x, y, dst->x, 5};
        } else {
            vert_rect = { x, y, 5, dst->y };
            horizontal_rect = { x, y + dst->y - 5, dst->x , 5};
        } 
    }

    if (first == 1) {
        vert_rect = { x + width - 5, y, 5, height };
        horizontal_rect = { x, y, width, 5};
    } else if (first == 2) {
        vert_rect = { x, y, 5, height };
        horizontal_rect = { x, y + height - 5, width, 5};
    } else {
        //neither has been set so don't draw anyting
        return;
    }

    if (*src->p_src) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    
    
    SDL_RenderFillRect(renderer, &vert_rect);
    SDL_RenderFillRect(renderer, &horizontal_rect);
}

void Wire::connect(Gate* gate, int idx) {
    gate->p_inputs[idx] = new Gate();
    dst = gate;
}
