#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "gate.h"
#include "input.h"
#include "wire.h"
#include "id.h"

class Simulation {
public:
    IdGenerator generator; 
    std::vector<Gate*> gates;
    std::vector<Input*> inputs;
    std::vector<Wire*> wires;
    std::vector<Light*> lights;
    
    Simulation();
    Simulation(std::vector<Gate*> &given_gates);
    void add_gate(std::string type, int x, int y, uint32_t id = 0);
    void add_input(int x, int y, uint32_t id = 0);
    void add_wire(Wire* wire);
    void add_light(int x, int y, uint32_t id = 0);
    void draw(SDL_Renderer* renderer);
    void simulate();
    void clear();
    void save_state();
    void load_state();
};
