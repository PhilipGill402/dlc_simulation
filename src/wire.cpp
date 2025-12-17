#include "wire.h"
#include <iostream>

Wire::Wire(Input* given_input) {
    input = given_input;
    src_gate = nullptr;
    dst_gate = nullptr;
    dst_idx = -1;
    
    start = { input->x, input->y };
    end = { input->x, input->y };

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire(Gate* gate) {
    input = nullptr;
    src_gate = gate;
    dst_gate = nullptr;
    dst_idx = -1;

    start = { src_gate->x, src_gate->y };
    end = { src_gate->x, src_gate->y };

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire(Wire* wire) {
    if (wire->input) {
        input = wire->input;
        src_gate = nullptr;
    } else if (wire->src_gate) {
        input = nullptr;
        src_gate = wire->src_gate;
    } else {
        input = nullptr;
        src_gate = nullptr;
    }
    
    dst_idx = -1;
    
    start = { wire->end.x, wire->end.y };
    end = { wire->end.x, wire->end.y };
    
    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

void Wire::draw(SDL_Renderer* renderer) {
    //sets the color of the wire based on the value of what it is connected to 
    if (src_gate) {
        if (src_gate->out) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
    } else if (input) {
        if (input->val) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    if (dst_gate) {
        end = { dst_gate->pin_in[dst_idx].x, dst_gate->pin_in[dst_idx].y };
    }
    
    SDL_Point elbow;
    int width = 0;
    int height = 0;

    if (first == 1) {
        elbow = { end.x, start.y };
        height = 5;
    } else if (first == 2) {
        elbow = { start.x, end.y };
        width = 5;
    } else {
        //neither has been set so don't draw anyting
        return;
    }
    
    SDL_Rect start_rect = { start.x - (width / 2), start.y - (height / 2), elbow.x - start.x + width , elbow.y - start.y + height };
    //swap height and width 
    SDL_Rect end_rect = { elbow.x - height, elbow.y - width, end.x - elbow.x + height, end.y - elbow.y + width };

    SDL_RenderFillRect(renderer, &start_rect);
    SDL_RenderFillRect(renderer, &end_rect);
    
    /*
    SDL_RenderDrawLine(renderer, start.x, start.y, elbow.x, elbow.y);
    SDL_RenderDrawLine(renderer, elbow.x, elbow.y, end.x, end.y);
    */
}

void Wire::connect(Gate* gate, int idx) {
    dst_gate = gate;
    dst_idx = idx;
    end = { gate->pin_in[idx].x, gate->pin_in[idx].y };
}

SDL_Rect (&Wire::get_rects())[2] {
    SDL_Rect rects[2]; 
    SDL_Point elbow;
    int width = 0;
    int height = 0;

    if (first == 1) {
        elbow = { end.x, start.y };
        height = 5;
    } else if (first == 2) {
        elbow = { start.x, end.y };
        width = 5;
    }

    rects[0] = { start.x - (width / 2), start.y - (height / 2), elbow.x - start.x + width , elbow.y - start.y + height };
    //swap height and width 
    rects[1] = { elbow.x - height, elbow.y - width, end.x - elbow.x + height, end.y - elbow.y + width };

    return rects;
}
