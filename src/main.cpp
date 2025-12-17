#include "main.h"

bool point_in_circle(SDL_Point point, int circle_x, int circle_y, int radius) {
    if ((point.x - circle_x) * (point.x - circle_x) + (point.y - circle_y) * (point.y - circle_y) < radius * radius) {
        return true;
    }

    return false;
}

constexpr int drag_threshold = 4;

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::perror("error initializeing SDL: ");
        std::cout << SDL_GetError() << "\n";

        return 1;
    }
        
    //width and height constants
    constexpr int width = 800;
    constexpr int height = 600;

    SDL_Window* win = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); 
    
    Input* input = new Input(100, 100);

    Simulation sim = Simulation();
    sim.add_input(input);

    //sets backround to grey
    SDL_SetRenderDrawColor(rend, 110, 110, 110, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
    
    //variable setup for event handling
    SDL_Point mouse_pos;
    SDL_Point offset_pos;
    SDL_Point down_pos;
    bool close = false;
    bool mouse_button_down = false;
    bool dragging = false;
    Gate* selected_gate = nullptr;
    Input* selected_input = nullptr;
    Wire* new_wire = nullptr;

    while (!close) {
        SDL_Event event; 
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_pos = { event.motion.x, event.motion.y };
                    if (mouse_button_down && (selected_gate || selected_input)) {
                        int dx = mouse_pos.x - down_pos.x;
                        int dy = mouse_pos.y - down_pos.y;
                        if (!dragging) {
                            if (dx*dx + dy*dy >= drag_threshold*drag_threshold) {
                                dragging = true; 
                            }
                        } else {
                            if (selected_gate) {
                                selected_gate->x = mouse_pos.x - offset_pos.x;
                                selected_gate->y = mouse_pos.y - offset_pos.y;
                            } else if (new_wire) {
                                //if dx is greater than dy then we want to elongate the horizontal rect, else we want to elongate the vertical rect 
                                if (dx > dy) {
                                    //if the vertical rect does not exist yet, then we want the first rect to be the horizontal one
                                    if (new_wire->height == 0) {
                                        new_wire->first = 1;
                                    }
                                    
                                    new_wire->width = mouse_pos.x - selected_input->x;
                                } else {
                                    //if the horizontal rect does not exist yet, then we want the first rect to be the vertical one
                                    if (new_wire->width == 0) {
                                        new_wire->first = 2;
                                    }

                                    new_wire->height = mouse_pos.y - selected_input->y;
                                } 
                            }
                            
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (mouse_button_down && event.button.button == SDL_BUTTON_LEFT) {
                        if (!dragging) {
                            for (Input* input : sim.inputs) {
                                if (point_in_circle(mouse_pos, input->x, input->y, 20)) {
                                    input->toggle(); 
                                }
                            }
                        }
                    }
                    
                    mouse_button_down = false;
                    dragging = false;
                    selected_gate = nullptr;
                    new_wire = nullptr;
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_button_down = true;
                        dragging = false; 
                        down_pos = { event.button.x, event.button.y };

                        for (Gate* gate : sim.gates) {
                            if (SDL_PointInRect(&mouse_pos, &gate->rect)) {
                                selected_gate = gate;

                                offset_pos.x = mouse_pos.x - gate->x;
                                offset_pos.y = mouse_pos.y - gate->y;
                                break;
                            }
                        }

                        for (Wire* wire : sim.wires) {
                            std::cout << "MOUSE X: " << mouse_pos.x << "\n";
                            std::cout << "MOUSE Y: " << mouse_pos.y << "\n";
                            std::cout << "WIRE X: " << wire->x + wire->width << "\n";
                            std::cout << "WIRE Y: " << wire->y + wire->height << "\n";
                            if (point_in_circle(mouse_pos, wire->x + wire->width, wire->y + wire->height, 10)) {
                                new_wire = new Wire(wire->src);
                                sim.add_wire(new_wire);
                            }
                            
                        }

                        for (Input* input : sim.inputs) {
                            if (point_in_circle(mouse_pos, input->x, input->y, 10)) {
                                new_wire = new Wire(input);
                                selected_input = input;

                                sim.add_wire(new_wire);
                                
                            }
                        }

                        
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_a) {
                        And* gate = new And(true, true, width/2, height/2);
                        sim.add_gate(gate);
                    } else if (event.key.keysym.sym == SDLK_o) {
                        Or* gate = new Or(true, true, width/2, height/2);
                        sim.add_gate(gate);
                    } else if (event.key.keysym.sym == SDLK_n) {
                        Not* gate = new Not(true, width/2, height/2);
                        sim.add_gate(gate);
                    }
                    break;
                default:
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(rend, 110, 110, 110, 255);   
        SDL_RenderClear(rend);

        sim.draw(rend);

        SDL_RenderPresent(rend); 
    }

    
    //cleanup
    SDL_DestroyRenderer(rend); 
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
