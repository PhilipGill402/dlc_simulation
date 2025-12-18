#pragma once
#include <SDL.h>
#include <vector>
#include "gate.h"
#include "input.h"
#include "wire.h"

class Simulation {
public:
    std::vector<Gate*> gates;
    std::vector<Input*> inputs;
    std::vector<Wire*> wires;
    std::vector<Light*> lights;
    
    Simulation();
    Simulation(std::vector<Gate*> &given_gates);
    void add_gate(Gate* gate);
    void add_input(Input* input);
    void add_wire(Wire* wire);
    void add_light(Light* light);
    void draw(SDL_Renderer* renderer);
    void simulate();
    void clear();
};
