#include "gate.h"

std::string Gate::to_string() {
    std::stringstream gate;
    gate << "Gate: (" << inputs[0] << ", " << inputs[1] << ") -> " << output << "\n";

    return gate.str();
}

//AND
And::And(bool input1, bool input2, int given_x, int given_y) {
    inputs[0] = input1;
    inputs[1] = input2;

    x = given_x;
    y = given_y;
    
    //sets output correctly
    evaluate();
}

std::string And::to_string() {
    std::stringstream gate;
    gate << "And: (" << inputs[0] << ", " << inputs[1] << ") -> " << output << "\n";

    return gate.str();
}

void And::evaluate() {
    output = inputs[0] && inputs[1];
}

void And::draw(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 100;
    rect.h = 50;

    //AND is green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

//OR
Or::Or(bool input1, bool input2, int given_x, int given_y) {
    inputs[0] = input1;
    inputs[1] = input2;
    
    x = given_x;
    y = given_y;

    //sets output correctly
    evaluate();
}

std::string Or::to_string() {
    std::stringstream gate;
    gate << "Or: (" << inputs[0] << ", " << inputs[1] << ") -> " << output << "\n";

    return gate.str();
}

void Or::evaluate() {
    output = inputs[0] || inputs[1];
}

//NOT
Not::Not(bool input, int given_x, int given_y) {
    inputs[0] = input;
    
    x = given_x;
    y = given_y;

    //sets output correctly
    evaluate();
}

std::string Not::to_string() {
    std::stringstream gate;
    gate << "Not: (" << inputs[0] << ") -> " << output << "\n";

    return gate.str();
}

void Not::evaluate() {
    output = !inputs[0];
}


