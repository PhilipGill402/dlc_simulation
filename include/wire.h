#pragma once
#include <SDL.h>
#include <array>
#include "input.h"
#include "gate.h"
#include "light.h"
#include "constants.h"

//included to avoid circular dependencies
class Simulation;

class Wire {
public:
    uint32_t id;

    //denotes the type of the source of the wire 
    ObjectType src_type;

    //id of the source
    uint32_t src_id;
    
    //denotes the type of the destination of the wire
    ObjectType dst_type;
    
    //id of the destination and index of the pin (if applicable)
    uint32_t dst_id;
    int dst_idx;
    
    //we are drawing between these two points
    SDL_Point start;
    SDL_Point end;

    //0 = not set
    //1 = horizontal first
    //2 = vertical first
    int first;

    Wire(Input* given_input, uint32_t given_id = 0);
    Wire(Gate* gate, uint32_t given_id = 0);
    Wire(Wire* wire, uint32_t given_id = 0);
    Wire();

    void draw(SDL_Renderer* renderer, const Simulation* sim);
    void connect(Gate* gate, int idx);
    void connect(Light* light);
    std::array<SDL_Rect, 2> get_rects();

    std::string to_string();
};
