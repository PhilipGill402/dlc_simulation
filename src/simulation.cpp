#include "simulation.h"
#include <iostream>

using json = nlohmann::json;

Simulation::Simulation() {
    gates = {};
    inputs = {};
    wires = {};
    lights = {};
}

Simulation::Simulation(std::vector<Gate*> &given_gates) {
    gates = given_gates;
}

void Simulation::add_gate(Gate* gate) {
    gates.push_back(gate);
}

void Simulation::add_input(Input* input) {
    inputs.push_back(input);
}

void Simulation::add_wire(Wire* wire) {
    wires.push_back(wire);
}

void Simulation::add_light(Light* light) {
    lights.push_back(light);
}

void Simulation::draw(SDL_Renderer* renderer) {
    for (Gate* gate : gates) {
        gate->draw(renderer);
    }

    for (Input* input : inputs) {
        input->draw(renderer);
    }

    for (Wire* wire : wires) {
        wire->draw(renderer);
    }

    for (Light* light : lights) {
        light->draw(renderer);
    }
}

void Simulation::simulate() {
    for (Wire* wire : wires) {
        bool val = false;
        if (wire->input) {
            val = wire->input->val;
        } else if (wire->src_gate) {
            val = wire->src_gate->out;
        }

        if (wire->dst_gate) {
            wire->dst_gate->pin_in[wire->dst_idx] = val; 
        } else if (wire->dst_light) {
            wire->dst_light->pin_in.value = val;
        }
    }

    for (Gate* gate : gates) {
        gate->evaluate();
    }

    for (Light* light : lights) {
        light->evaluate();
    }
}

void Simulation::clear() {
    for (Gate* gate : gates) {
        delete gate;
    }
    gates.clear();
    for (Input* input : inputs) {
        delete input;
    }
    inputs.clear();
    for (Wire* wire : wires) {
        delete wire;
    }
    wires.clear();
    for (Light* light : lights) {
        delete light;
    }
    lights.clear();
}

void Simulation::save_state() {
    std::ofstream save_file;
    save_file.open("save/save_file.json", std::ofstream::trunc);
    
    if (!save_file.is_open()) {
        std::cout << "failed to create save file\n";
        return;
    }

    json contents;
    for (Gate* gate : gates) {
        std::string type;
        if (dynamic_cast<And*>(gate)) {
            type = "And";
        } else if (dynamic_cast<Or*>(gate)) {
            type = "Or";
        } else if (dynamic_cast<Not*>(gate)) {
            type = "Not";
        } else {
            std::cout << "failed to recognize gate type\n";
            return;
        }
        contents["Gate"]["type"] = type;
        contents["Gate"]["x"] = gate->x;
        contents["Gate"]["y"] = gate->y;
        contents["Gate"]["w"] = gate->w;
        contents["Gate"]["h"] = gate->h;
    }

    std::string save_state = contents.dump();
    save_file << save_state;
}
