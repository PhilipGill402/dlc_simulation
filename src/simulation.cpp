#include "simulation.h"
#include <iostream>

using json = nlohmann::json;

using json = nlohmann::json;

Simulation::Simulation() {
    generator = IdGenerator(); 
    gates = {};
    inputs = {};
    wires = {};
    lights = {};
}

Simulation::Simulation(std::vector<Gate*> &given_gates) {
    gates = given_gates;
}

void Simulation::add_gate(std::string type, int x, int y, uint32_t id) {
    Gate* gate;
    if (type == "And") {
        gate = new And(x, y); 
    } else if (type == "Or") {
        gate = new Or(x, y);
    } else if (type == "Not") {
        gate = new Not(x, y);
    } else {
        std::cout << "unrecognized gate type\n";
        return; 
    }
    
    if (id == 0){
        gate->id = generator.make_id();
    } else {
        gate->id = id;
    }

    gates.push_back(gate);
}

void Simulation::add_input(int x, int y, uint32_t id) {
    Input* input = new Input(x, y);
    
    if (id == 0){
        input->id = generator.make_id();
    } else {
        input->id = id;
    }

    inputs.push_back(input);
}

void Simulation::add_wire(Wire* wire) {
    wires.push_back(wire);
}

void Simulation::add_light(int x, int y, uint32_t id) {
    Light* light = new Light(x, y);
    
    if (id == 0){
        light->id = generator.make_id();
    } else {
        light->id = id;
    }

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
    
    json file_contents;

    std::vector<json> gates_data;
    for (Gate* gate : gates) {
        json gate_data;     
    
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
        
        gate_data["type"] = type;
        gate_data["id"] = gate->id;
        gate_data["x"] = gate->x;
        gate_data["y"] = gate->y;
        gate_data["w"] = gate->w;
        gate_data["h"] = gate->h;
        gates_data.push_back(gate_data);
    }

    //handles inputs
    std::vector<json> inputs_data;
    for (Input* input : inputs) {
        json input_data;
        
        input_data["id"] = input->id;
        input_data["x"] = input->x;
        input_data["y"] = input->y;
        input_data["value"] = input->val;
        inputs_data.push_back(input_data);    
    } 
    
    //handles lights
    std::vector<json> lights_data;
    for (Light* light : lights) {
        json light_data;
        
        light_data["id"] = light->id;
        light_data["x"] = light->x;
        light_data["y"] = light->y;
        light_data["value"] = light->val;
        lights_data.push_back(light_data);    
    }

    file_contents["Gates"] = gates_data;
    file_contents["Inputs"] = inputs_data;
    file_contents["Lights"] = lights_data;

    std::string save_state = file_contents.dump(4);
    save_file << save_state;
}

void Simulation::load_state() {
    std::ifstream save_file("save/save_file.json");

    if (!save_file.is_open()) {
        std::cout << "failed to open save file\n";
        return;
    }

    json file_content;
    save_file >> file_content;
    
    std::vector<json> gates_data = file_content.at("Gates");
    std::vector<json> inputs_data = file_content.at("Inputs");
    std::vector<json> lights_data = file_content.at("Lights");
    
    //handles gates
    for (json gate : gates_data) {
        std::string type = gate["type"];
        int id = gate["id"];
        int x = gate["x"];
        int y = gate["y"];
        add_gate(type, x, y, id);
    }

    //handles inputs
    for (json input : inputs_data) {
        int id = input["id"];
        int x = input["x"];
        int y = input["y"];
        add_input(x, y, id);
    }

    //handles lights 
    for (json light : lights_data) {
        int id = light["id"];
        int x = light["x"];
        int y = light["y"];
        add_light(x, y, id);
    }
}
