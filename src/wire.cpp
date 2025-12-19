#include "wire.h"
#include "simulation.h"
#include <iostream>
#include <sstream>

Wire::Wire(Input* given_input, uint32_t given_id) {
    id = given_id; 
    src_type = ObjectType::INPUT;
    src_id = given_input->id;
    dst_type = ObjectType::NONE;
    dst_id = 0;
    dst_idx = -1;
    
    start = { given_input->x, given_input->y };
    end = { given_input->x, given_input->y };

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire(Gate* gate, uint32_t given_id) {
    id = given_id; 
    src_type = ObjectType::GATE;
    src_id = gate->id;
    dst_type = ObjectType::NONE;
    dst_id = 0;
    dst_idx = -1;

    start = { gate->pin_out.x, gate->pin_out.y };
    end = { gate->pin_out.x, gate->pin_out.y };

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire(Wire* wire, uint32_t given_id) {
    id = given_id;
    src_type = ObjectType::WIRE;
    src_id = wire->id;
    dst_type = ObjectType::NONE;
    dst_id = 0;
    dst_idx = -1;
    
    start = { wire->end.x, wire->end.y };
    end = { wire->end.x, wire->end.y };
    
    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    first = 0;
}

Wire::Wire() {
    id = 0;
    src_type = ObjectType::NONE;
    src_id = 0;
    dst_type = ObjectType::NONE;
    dst_id = 0;
    dst_idx = -1;
    
    start = { 0, 0 };
    end = { 0, 0 };

    first = 0;
}

void Wire::draw(SDL_Renderer* renderer, const Simulation* sim) {
    //sets the color of the wire based on the value of what it is connected to
    if (src_type == ObjectType::GATE) {
        Gate* src_gate; 
        if (!(src_gate = sim->get_gate(src_id))) {
            std::cout << "failed to get source gate\n"; 
            return;
        }

        start = { src_gate->pin_out.x, src_gate->pin_out.y }; 
        
        if (src_gate->out) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

    } else if (src_type == ObjectType::INPUT) {
        Input* input;
        if (!(input = sim->get_input(src_id))) {
            std::cout << "failed to get input\n";
            return;
        }
        
        start = { input->x, input->y };

        if (input->val) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

    } else if (src_type == ObjectType::WIRE) {
        Wire* connecting_wire;
        if (!(connecting_wire = sim->get_wire(src_id))) {
            std::cout << "failed to get wire\n";
            return;
        }
        start = { connecting_wire->end.x, connecting_wire->end.y };

    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    
    if (dst_type == ObjectType::GATE && (dst_idx == 0 || dst_idx == 1)) {
        Gate* dst_gate;
        if (!(dst_gate = sim->get_gate(dst_id))) {
            std::cout << "failed to get destination gate\n";
            return;
        }

        end = { dst_gate->pin_in[dst_idx].x, dst_gate->pin_in[dst_idx].y };
    } else if (dst_type == ObjectType::LIGHT) {
        Light* dst_light;
        if (!(dst_light = sim->get_light(dst_id))) {
            std::cout << "failed to get destination light\n";
            return;
        }

        end = { dst_light->x, dst_light->y };
    }

    SDL_Point elbow;
    int width = 0;
    int height = 0;

    if (first == 1) {
        elbow = { end.x, start.y };
        height = 5;
    } else if (first == 2) {
        elbow = { start.x, end.y };
        width = 5;
    } else {
        //neither has been set so don't draw anyting
        return;
    }
    
    SDL_Rect start_rect = { start.x - (width / 2), start.y - (height / 2), elbow.x - start.x + width , elbow.y - start.y + height };
    //swap height and width 
    SDL_Rect end_rect = { elbow.x - height, elbow.y - width, end.x - elbow.x + height, end.y - elbow.y + width };

    SDL_RenderFillRect(renderer, &start_rect);
    SDL_RenderFillRect(renderer, &end_rect);
    
    /*
    SDL_RenderDrawLine(renderer, start.x, start.y, elbow.x, elbow.y);
    SDL_RenderDrawLine(renderer, elbow.x, elbow.y, end.x, end.y);
    */
}

void Wire::connect(Gate* gate, int idx) {
    dst_type = ObjectType::GATE;
    dst_id = gate->id;
    dst_idx = idx;
    end = { gate->pin_in[idx].x, gate->pin_in[idx].y };
}

void Wire::connect(Light* light) {
    dst_type = ObjectType::LIGHT;
    dst_id = light->id;
}

std::array<SDL_Rect, 2> Wire::get_rects() {
    std::array<SDL_Rect, 2> rects{}; 
    SDL_Point elbow;
    int width = 0;
    int height = 0;
    
    if (first == 1) {
        elbow = { end.x, start.y };
        height = 5;
    } else if (first == 2) {
        elbow = { start.x, end.y };
        width = 5;
    } else {
        return rects;
    }

    rects[0] = { start.x - (width / 2), start.y - (height / 2), elbow.x - start.x + width , elbow.y - start.y + height };
    //swap height and width 
    rects[1] = { elbow.x - height, elbow.y - width, end.x - elbow.x + height, end.y - elbow.y + width };

    return rects;
}

bool Wire::get_val(Simulation* sim) {
    bool val = false;
        
    if (src_type == ObjectType::INPUT) {
        Input* input;
        if (!(input = sim->get_input(src_id))) {
            std::cout << "failed to get source input\n";
            return false;
        }

        val = input->val;
    } else if (src_type == ObjectType::GATE) {
        Gate* gate;
        if (!(gate = sim->get_gate(src_id))) {
            std::cout << "failed to get source gate\n";
            return false;
        }

        val = gate->pin_out.value;
    } else if (src_type == ObjectType::WIRE) {
        Wire* connected_wire;
        if (!(connected_wire = sim->get_wire(src_id))) {
            std::cout << "failed to get source wire\n";
            return false;
        }

        val = connected_wire->get_val(sim);
    }

    return val;
}

void Wire::evaluate(Simulation* sim) {
    bool val = get_val(sim); 
        
    if (dst_type == ObjectType::GATE) {
        Gate* dst_gate;
        if (!(dst_gate = sim->get_gate(dst_id))) {
            std::cout << "failed to get destination gate\n";
            return;
        }

        dst_gate->pin_in[dst_idx] = val; 
    } else if (dst_type == ObjectType::LIGHT) {
        Light* dst_light;
        if (!(dst_light = sim->get_light(dst_id))) {
            std::cout << "failed to get destination light\n";
            return;
        }

        dst_light->pin_in.value = val;
    }
}

std::string Wire::to_string() {
    std::stringstream str;
    str << "Wire " << id;
    return str.str();
}
