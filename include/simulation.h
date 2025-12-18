#pragma once
#include <SDL.h>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
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
    std::unordered_map<uint32_t, Gate*> gate_map;
    std::unordered_map<uint32_t, Input*> input_map;
    std::unordered_map<uint32_t, Wire*> wire_map;
    std::unordered_map<uint32_t, Light*> light_map;
    
    Simulation();
    void add_gate(std::string type, int x, int y, uint32_t id = 0);
    void add_input(int x, int y, uint32_t id = 0);
    Wire* add_wire(Wire* wire, uint32_t id = 0);
    Wire* add_wire(Input* input, uint32_t id = 0);
    Wire* add_wire(Gate* gate, uint32_t id = 0);
    void add_light(int x, int y, uint32_t id = 0);
    Gate* get_gate(uint32_t id) const;
    Input* get_input(uint32_t id) const;
    Wire* get_wire(uint32_t id) const;
    Light* get_light(uint32_t id) const;
    void draw(SDL_Renderer* renderer);
    void simulate();
    void clear();
    void save_state();
    void load_state();
};
