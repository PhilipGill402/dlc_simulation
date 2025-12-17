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
    
    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    int first;

    Wire(Input* given_input);
    Wire(Gate* gate);
};
