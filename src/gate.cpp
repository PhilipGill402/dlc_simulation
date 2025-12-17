#include "gate.h"

/* *** Helper Functions *** */
void draw_half_circle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = -radius; w <= 0; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius*radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

/* *** Gate Functions *** */
std::string Gate::to_string() {
    std::stringstream gate;
    gate << "Gate: (" << inputs[0] << ", " << inputs[1] << ") -> " << output << "\n";

    return gate.str();
}

void Gate::draw(SDL_Renderer* renderer) {
    rect.x = x;
    rect.y = y;

    //General is black 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

/* *** AND Functions *** */
And::And(bool input1, bool input2, int given_x, int given_y) {
    inputs[0] = input1;
    inputs[1] = input2;
    
    evaluated = false;

    x = given_x;
    y = given_y;

    rect.x = x;
    rect.y = y;
    rect.w = block_width;
    rect.h = block_height;
    
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
    rect.x = x;
    rect.y = y;
    
    //input nodes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw_half_circle(renderer, x, y+15, 10);
    draw_half_circle(renderer, x, y+40, 10);

    //AND is green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

/* *** OR Functions *** */
Or::Or(bool input1, bool input2, int given_x, int given_y) {
    inputs[0] = input1;
    inputs[1] = input2;
    
    evaluated = false;

    x = given_x;
    y = given_y;
    
    rect.x = x;
    rect.y = y;
    rect.w = block_width;
    rect.h = block_height;

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

void Or::draw(SDL_Renderer* renderer) {
    rect.x = x;
    rect.y = y;
    
    //input nodes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw_half_circle(renderer, x, y+15, 10);
    draw_half_circle(renderer, x, y+40, 10);

    //OR is blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
 
/* *** NOT Functions *** */
Not::Not(bool input, int given_x, int given_y) {
    inputs[0] = input;

    evaluated = false;

    x = given_x;
    y = given_y;
    
    rect.x = x;
    rect.y = y;
    rect.w = block_width;
    rect.h = block_height;

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

void Not::draw(SDL_Renderer* renderer) {
    rect.x = x;
    rect.y = y;
    
    //input nodes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw_half_circle(renderer, x, y+27, 10);

    //NOT is red 
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
