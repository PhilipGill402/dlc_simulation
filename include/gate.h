#pragma once
#include <SDL.h>
#include <sstream>

constexpr int block_width = 100;
constexpr int block_height = 55;

class Gate{
public:
    //boolean representations of inputs and outputs
    bool inputs[2];
    bool output;
    
    //pointers to inputs and outputs
    Gate** p_inputs;
    Gate** p_outputs;
    int num_inputs;
    

    //set to true once the gates output has been evaluated
    bool evaluated;

    //physical position
    int x;
    int y;
    
    //rect for drawing the gate
    SDL_Rect rect;

    virtual ~Gate() = default;
    Gate(bool val);
    Gate();
    virtual void evaluate();
    virtual std::string to_string();
    virtual void draw(SDL_Renderer* renderer);
};

class And : public Gate{
public:
    And(bool input1, bool input2, int given_x, int given_y);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
};

class Or : public Gate{
public:
    Or(bool input1, bool input2, int given_x, int given_y);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
};

class Not : public Gate{
public:
    Not(bool input, int given_x, int given_y);
    std::string to_string() override;   
    void evaluate() override;
    void draw(SDL_Renderer* renderer) override;
};
