#pragma once
#include <SDL.h>
#include <sstream>
#include <vector>
#include "constants.h"

class Pin {
public:
    int x;
    int y;
    bool value;
    
    Pin();
    Pin(bool given_value);
    Pin(int given_x, int given_y, bool given_value);
    std::string to_string();
    void draw(SDL_Renderer* renderer, bool is_left);
};

class Gate{
public:
    //id to distinguish each gate
    uint32_t id;

    //boolean representations of inputs and outputs
    bool in[2];
    bool out;
    
    //holds the pins of the gate
    Pin pin_in[2];
    Pin pin_out;
     
    //physical position
    int x;
    int y;
    int w;
    int h;
    
    virtual ~Gate() = default;
    Gate();
    virtual void evaluate() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual std::string to_string();
    virtual void update_pins();
    void draw_pins(SDL_Renderer* renderer);
    SDL_Rect get_rect();
};

class And : public Gate{
public:
    And(int given_x, int given_y, uint32_t given_id = 0);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
};

class Or : public Gate{
public:
    Or(int given_x, int given_y, uint32_t given_id = 0);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
};

class Not : public Gate{
public:
    Not(int given_x, int given_y, uint32_t given_id = 0);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
    void update_pins() override;
};
