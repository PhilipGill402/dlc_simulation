#include "simulation.h"

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
    gates.clear();
    inputs.clear();
    wires.clear();
    lights.clear();
}
