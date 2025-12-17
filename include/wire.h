#pragma once
#include <SDL.h>
#include <vector>
#include "input.h"
#include "gate.h"

constexpr int wire_height = 5;

class Wire {
public:
    int x;
    int y;
    
    //0 = not assigned
    //1 = width
    //2 = height
    int first;
    
    int width;
    int height;
    
    Gate* src;

    Wire(Input* input);
    Wire(Gate* gate);
    void draw(SDL_Renderer* renderer);
    void connect(Input* input, Gate* gate);
    void connect(Gate* input, Gate* gate);
};
