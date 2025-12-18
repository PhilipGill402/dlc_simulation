#include "gate.h"

/* *** Helper Functions *** */
void draw_left_half_circle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = -radius; w <= 0; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius*radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

void draw_right_half_circle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius*radius) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

/* *** Pin Functions *** */
Pin::Pin() {
    x = 0;
    y = 0;
    value = false;
}

Pin::Pin(int given_x, int given_y, bool given_value) {
    x = given_x;
    y = given_y;
    value = given_value;
}

Pin::Pin(bool given_value) {
    x = 0;
    y = 0;
    value = given_value;
}

std::string Pin::to_string() {
    return value ? "true" : "false";
}

void Pin::draw(SDL_Renderer* renderer, bool is_left) {
    if (is_left) {
        draw_left_half_circle(renderer, x, y, PIN_RADIUS);
    } else {
        draw_right_half_circle(renderer, x, y, PIN_RADIUS);
    }
}

/* *** Gate Functions *** */
Gate::Gate() {
    id = 0;  

    //boolean representations of inputs and outputs
    in[0] = false;
    in[1] = false;
    out = false;
    
    //physical position
    x = 0;
    y = 0;
    w = 100;
    h = 50;
}

std::string Gate::to_string() {
    std::stringstream gate;
    gate << "Gate " << id << ": (" << in[0] << ", " << in[1] << ") -> " << out;

    return gate.str();
}

void Gate::update_pins() {
    pin_in[0].x = x;
    pin_in[0].y = y + h/3;
    pin_in[1].x = x;
    pin_in[1].y = y + 2*h/3;
    pin_out.x = x+w;
    pin_out.y = y + h/2;
}

void Gate::draw_pins(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    pin_in[0].draw(renderer, true);
    pin_in[1].draw(renderer, true);
    pin_out.draw(renderer, false);
}

SDL_Rect Gate::get_rect() {
    return SDL_Rect{ x, y, w, h };
}

/* *** AND Functions *** */
And::And(int given_x, int given_y, uint32_t given_id) : Gate() {
    id = given_id; 
    x = given_x;
    y = given_y;
    update_pins();
}

std::string And::to_string() {
    std::stringstream gate;
    gate << "And " << id << ": (" << in[0] << ", " << in[1] << ") -> " << out;

    return gate.str();
}

void And::evaluate() {
    out = pin_in[0].value && pin_in[1].value;
    in[0] = pin_in[0].value;
    in[1] = pin_in[1].value;
    pin_out.value = out;
}

void And::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, w, h };
    update_pins(); 

    draw_pins(renderer);    

    //AND is green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

/* *** OR Functions *** */
Or::Or(int given_x, int given_y, uint32_t given_id) : Gate() {
    id = given_id; 
    x = given_x;
    y = given_y;
    update_pins(); 
}

std::string Or::to_string() {
    std::stringstream gate;
    gate << "Or " << id << ": (" << in[0] << ", " << in[1] << ") -> " << out;

    return gate.str();
}

void Or::evaluate() {
    out = pin_in[0].value || pin_in[1].value;
    in[0] = pin_in[0].value;
    in[1] = pin_in[1].value;
    pin_out.value = out;
}

void Or::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, w, h }; 
    update_pins();

    //input nodes
    draw_pins(renderer); 

    //OR is blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
 
/* *** NOT Functions *** */
Not::Not(int given_x, int given_y, uint32_t given_id) : Gate() {
    id = given_id; 
    x = given_x;
    y = given_y;

    update_pins(); 
}

void Not::update_pins() {
    pin_in[0].x = x;
    pin_in[0].y = y + h/2;
    pin_out.x = x+w;
    pin_out.y = y + h/2;
}

std::string Not::to_string() {
    std::stringstream gate;
    gate << "Not " << id << ": (" << in[0] << ") -> " << out;

    return gate.str();
}

void Not::evaluate() {
    out = !pin_in[0].value;
    in[0] = pin_in[0].value;
    pin_out.value = out;
}

void Not::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, w, h };
    update_pins();

    //input nodes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw_left_half_circle(renderer, x, y + h/2, PIN_RADIUS);
    draw_right_half_circle(renderer, x+w, y + h/2, PIN_RADIUS);

    //NOT is red 
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
