#pragma once
#include <SDL.h>
#include <vector>
#include "input.h"
#include "gate.h"

constexpr int wire_height = 5;

class Wire {
public:
    Input* input;
    Gate* src_gate;
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
    SDL_Rect (&get_rects())[2]; 
};
