#include "simulation.h"

Simulation::Simulation() {
    gates = {};
    inputs = {};
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
}
