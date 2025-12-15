#pragma once
#include <SDL.h>
#include <sstream>

class Gate{
public:
    bool inputs[2];
    bool output;

    int x;
    int y;
    
    virtual ~Gate() = default;
    std::string to_string();
};

class And : public Gate{
public:
    And(bool input1, bool input2, int given_x, int given_y);
    std::string to_string();   
    void evaluate();
    void draw(SDL_Renderer* renderer);
};

class Or : public Gate{
public:
    Or(bool input1, bool input2, int given_x, int given_y);
    std::string to_string();   
    void evaluate();
    void draw(SDL_Surface* surface);
};

class Not : public Gate{
public:
    Not(bool input, int given_x, int given_y);
    std::string to_string();   
    void evaluate();
    void draw(SDL_Surface* surface);
};
