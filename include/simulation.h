#pragma once
#include <SDL.h>
#include <vector>
#include "gate.h"
#include "input.h"

class Simulation {
public:
    std::vector<Gate*> gates;
    std::vector<Input*> inputs;
    
    Simulation();
    Simulation(std::vector<Gate*> &given_gates);
    void add_gate(Gate* gate);
    void add_input(Input* input);
    void draw(SDL_Renderer* renderer);
};
