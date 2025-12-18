#pragma once
#include <SDL.h>
#include <array>
#include "input.h"
#include "gate.h"

constexpr int wire_height = 5;

class Wire {
public:
    //three possible sources of input 
    Input* input;
    Gate* src_gate;
    Wire* connecting_wire;
    
    //destination gate and index of the pin
    Gate* dst_gate;
    int dst_idx;
    
    //we are drawing between these two points
    SDL_Point start;
    SDL_Point end;

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    int first;

    Wire(Input* given_input);
    Wire(Gate* gate);
    Wire(Wire* wire);

    void draw(SDL_Renderer* renderer);
    void connect(Gate* gate, int idx);
    std::array<SDL_Rect, 2> get_rects();

    std::string to_string();
};
