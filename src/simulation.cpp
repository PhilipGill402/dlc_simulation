#include "simulation.h"
#include <iostream>

using json = nlohmann::json;

using json = nlohmann::json;

Simulation::Simulation() {
    generator = IdGenerator(); 
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

    gate_map[gate->id] = gate; 
    gates.push_back(gate);
}

void Simulation::add_input(int x, int y, uint32_t id) {
    Input* input = new Input(x, y);
    
    if (id == 0){
        input->id = generator.make_id();
    } else {
        input->id = id;
    }

    input_map[input->id] = input;
    inputs.push_back(input);
}

Wire* Simulation::add_wire(Wire* wire, uint32_t id) {
    Wire* new_wire = new Wire(wire);

    if (id == 0) {
        new_wire->id = generator.make_id();
    } else {
        new_wire->id = id;
    }
    
    wire_map[new_wire->id] = new_wire;
    wires.push_back(new_wire);

    return new_wire;
}

Wire* Simulation::add_wire(Input* input, uint32_t id) {
    Wire* wire = new Wire(input);

    if (id == 0) {
        wire->id = generator.make_id();
    } else {
        wire->id = id;
    }
    
    wire_map[wire->id] = wire;
    wires.push_back(wire);

    return wire;
}

Wire* Simulation::add_wire(Gate* gate, uint32_t id) {
    Wire* wire = new Wire(gate);

    if (id == 0) {
        wire->id = generator.make_id();
    } else {
        wire->id = id;
    }
    
    wire_map[wire->id] = wire;
    wires.push_back(wire);

    return wire;
}

void Simulation::add_light(int x, int y, uint32_t id) {
    Light* light = new Light(x, y);
    
    if (id == 0){
        light->id = generator.make_id();
    } else {
        light->id = id;
    }
    
    light_map[light->id] = light;
    lights.push_back(light);
}

Gate* Simulation::get_gate(uint32_t id) const {
    Gate* gate; 
    try {
        gate = gate_map.at(id);
    } catch (std::out_of_range){
        return nullptr;
    }
    return gate;
}

Input* Simulation::get_input(uint32_t id) const {
    Input* input; 
    try {
        input = input_map.at(id);
    } catch (std::out_of_range){
        return nullptr;
    }
    return input;
}

Wire* Simulation::get_wire(uint32_t id) const {
    Wire* wire; 
    try {
        wire = wire_map.at(id);
    } catch (std::out_of_range){
        return nullptr;
    }

    return wire;
}

Light* Simulation::get_light(uint32_t id) const {
    Light* light; 
    try {
        light = light_map.at(id);
    } catch (std::out_of_range){
        return nullptr;
    }

    return light;
}

void Simulation::draw(SDL_Renderer* renderer) {
    for (Gate* gate : gates) {
        gate->draw(renderer);
    }

    for (Input* input : inputs) {
        input->draw(renderer);
    }

    for (Wire* wire : wires) {
        wire->draw(renderer, this);
    }

    for (Light* light : lights) {
        light->draw(renderer);
    }
}

void Simulation::simulate() {
    for (Wire* wire : wires) {
        bool val = false;
        
        if (wire->src_type == ObjectType::INPUT) {
            Input* input;
            if (!(input = get_input(wire->src_id))) {
                std::cout << "failed to get source input\n";
                return;
            }

            val = input->val;
        } else if (wire->src_type == ObjectType::GATE) {
            Gate* gate;
            if (!(gate = get_gate(wire->src_id))) {
                std::cout << "failed to get source gate\n";
                return;
            }

            val = gate->pin_out.value;
        }
        
        if (wire->dst_type == ObjectType::GATE) {
            Gate* dst_gate;
            if (!(dst_gate = get_gate(wire->dst_id))) {
                std::cout << "failed to get destination gate\n";
                return;
            }

            dst_gate->pin_in[wire->dst_idx] = val; 
        } else if (wire->dst_type == ObjectType::LIGHT) {
            Light* dst_light;
            if (!(dst_light = get_light(wire->dst_id))) {
                std::cout << "failed to get destination light\n";
                return;
            }

            dst_light->pin_in.value = val;
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

    //handles wires
    std::vector<json> wires_data;
    for (Wire* wire : wires) {
        json wire_data;
        
        wire_data["id"] = wire->id;
        wire_data["src_type"] = wire->src_type;
        wire_data["src_id"] = wire->src_id;
        wire_data["dst_type"] = wire->dst_type;
        wire_data["dst_id"] = wire->dst_id;
        wire_data["dst_idx"] = wire->dst_idx;
        wire_data["first"] = wire->first;
        wire_data["start_x"] = wire->start.x;
        wire_data["start_y"] = wire->start.y;
        wire_data["end_x"] = wire->end.x;
        wire_data["end_y"] = wire->end.y;

        wires_data.push_back(wire_data);
    }

    file_contents["Gates"] = gates_data;
    file_contents["Inputs"] = inputs_data;
    file_contents["Lights"] = lights_data;
    file_contents["Wires"] = wires_data;

    std::string save_state = file_contents.dump(4);
    save_file << save_state;
}

void Simulation::load_state() {
    sim.clear(); 

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
    std::vector<json> wires_data = file_content.at("Wires");
    
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

    //handles wires
    for (json wire : wires_data) {
        int id = wire["id"];
        ObjectType src_type = wire["src_type"];
        int src_id = wire["src_id"];
        ObjectType dst_type = wire["dst_type"];
        int dst_id = wire["dst_id"];
        int dst_idx = wire["dst_idx"];
        int first = wire["first"];
        int start_x = wire["start_x"];
        int start_y = wire["start_y"];
        int end_x = wire["end_x"];
        int end_y = wire["end_y"];
         
        Wire* new_wire = new Wire();

        new_wire->id = id;
        new_wire->src_type = src_type;
        new_wire->src_id = src_id;
        new_wire->dst_type = dst_type;
        new_wire->dst_id = dst_id;
        new_wire->dst_idx = dst_idx;
        new_wire->first = first;
        new_wire->start = { start_x, start_y };
        new_wire->end = { end_x, end_y };

        wire_map[id] = new_wire;
        wires.push_back(new_wire);
    }
}
