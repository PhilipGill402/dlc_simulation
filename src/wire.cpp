#include "wire.h"
#include <iostream>
#include <sstream>

Wire::Wire(Input* given_input) {
    input = given_input;
    src_gate = nullptr;
    connecting_wire = nullptr;
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
    connecting_wire = nullptr;
    dst_gate = nullptr;
    dst_idx = -1;

    start = { src_gate->pin_out.x, src_gate->pin_out.y };
    end = { src_gate->pin_out.x, src_gate->pin_out.y };

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
    
    connecting_wire = wire;
    dst_gate = nullptr;
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
    
    if (connecting_wire) {
        start = { connecting_wire->end.x, connecting_wire->end.y };
    } else if (src_gate) {
        start = { src_gate->pin_out.x, src_gate->pin_out.y }; 
    }
    
    if (dst_gate && (dst_idx == 0 || dst_idx == 1)) {
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

std::array<SDL_Rect, 2> Wire::get_rects() {
    std::array<SDL_Rect, 2> rects{}; 
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
        return rects;
    }

    rects[0] = { start.x - (width / 2), start.y - (height / 2), elbow.x - start.x + width , elbow.y - start.y + height };
    //swap height and width 
    rects[1] = { elbow.x - height, elbow.y - width, end.x - elbow.x + height, end.y - elbow.y + width };

    return rects;
}

std::string Wire::to_string() {
    std::stringstream str;
    str << "Wire X:" << start.x << ", Y: " << start.y;
    return str.str();
}
