#include "main.h"

bool point_in_circle(SDL_Point point, int circle_x, int circle_y, int radius) {
    if ((point.x - circle_x) * (point.x - circle_x) + (point.y - circle_y) * (point.y - circle_y) < radius * radius) {
        return true;
    }

    return false;
}

bool point_in_left_half_circle(SDL_Point point, int circle_x, int circle_y, int radius) {
    if ((point.x - circle_x) * (point.x - circle_x) + (point.y - circle_y) * (point.y - circle_y) < radius * radius && point.x <= circle_x) {
        return true;
    }

    return false;
}

bool point_in_right_half_circle(SDL_Point point, int circle_x, int circle_y, int radius) {
    if ((point.x - circle_x) * (point.x - circle_x) + (point.y - circle_y) * (point.y - circle_y) < radius * radius && point.x >= circle_x) {
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
    
    Simulation sim = Simulation();

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
    Wire* selected_wire = nullptr;

    while (!close) {
        SDL_Event event; 
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_pos = { event.motion.x, event.motion.y };
                    if (mouse_button_down && (selected_gate || selected_input || selected_wire)) {
                        int dx = mouse_pos.x - down_pos.x;
                        int dy = mouse_pos.y - down_pos.y;
                        if (!dragging) {
                            if (dx*dx + dy*dy >= drag_threshold*drag_threshold) {
                                if (selected_input) {
                                    selected_wire = new Wire(selected_input);
                                    sim.add_wire(selected_wire);
                                } 
                                dragging = true; 
                            }
                        } else {
                            if (selected_gate) {
                                selected_gate->x = mouse_pos.x - offset_pos.x;
                                selected_gate->y = mouse_pos.y - offset_pos.y;
                            } else if (selected_wire) {
                                selected_wire->end.x = mouse_pos.x;
                                selected_wire->end.y = mouse_pos.y;
                                
                                if (selected_wire->first == 0 && dx*dx > dy*dy) {
                                    selected_wire->first = 1;
                                } else if (selected_wire->first == 0 && dy*dy > dx*dx){
                                    selected_wire->first = 2;
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
                                    break;
                                }
                            }
                        } else {
                            //only want to check gate connectors if we are holding onto a wire
                            if (selected_wire) {
                                //check to see if mouse is over a gate connector
                                for (Gate* gate : sim.gates) {
                                    if (point_in_left_half_circle(mouse_pos, gate->pin_in[0].x, gate->pin_in[0].y, 10)) {
                                        selected_wire->connect(gate, 0); 
                                    } else if (point_in_left_half_circle(mouse_pos, gate->pin_in[1].x, gate->pin_in[1].y, 10)) {
                                        selected_wire->connect(gate, 1); 
                                    }
                                }
                            }
                        }
                    }
                    
                    mouse_button_down = false;
                    dragging = false;
                    selected_gate = nullptr;
                    selected_input = nullptr;
                    selected_wire = nullptr;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_button_down = true;
                        dragging = false;
                        bool handled = false;
                        
                        mouse_pos = { event.button.x, event.button.y };
                        down_pos = mouse_pos;

                        for (Gate* gate : sim.gates) {
                            SDL_Rect rect = gate->get_rect();
                            if (SDL_PointInRect(&mouse_pos, &rect)) {
                                selected_gate = gate;
                                handled = true;

                                offset_pos.x = mouse_pos.x - gate->x;
                                offset_pos.y = mouse_pos.y - gate->y;
                                break;
                            } else if (point_in_right_half_circle(mouse_pos, gate->pin_out.x, gate->pin_out.y, 10)) {
                                selected_wire = new Wire(gate);
                                sim.add_wire(selected_wire);
                                handled = true;

                                break;
                            }
                        }

                        if (handled) {
                            break;
                        }

                        for (Wire* wire : sim.wires) {
                            //if you click at the end, create a new wire 
                            if (point_in_circle(mouse_pos, wire->end.x, wire->end.y, 10)) {
                                selected_wire = new Wire(wire);
                                sim.add_wire(selected_wire);
                                handled = true;
                                break;
                            }

                            std::array<SDL_Rect, 2> rects = wire->get_rects();

                            if (SDL_PointInRect(&mouse_pos, &rects[0]) || SDL_PointInRect(&mouse_pos, &rects[1])){
                                selected_wire = wire; 
                            }
                            
                        }

                        if (handled) {
                            break;
                        }

                        for (Input* input : sim.inputs) {
                            if (point_in_circle(mouse_pos, input->x, input->y, 10)) {
                                selected_input = input;
                            }
                        }

                        
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_a) {
                        And* gate = new And(mouse_pos.x, mouse_pos.y);
                        sim.add_gate(gate);
                    } else if (event.key.keysym.sym == SDLK_o) {
                        Or* gate = new Or(mouse_pos.x, mouse_pos.y);
                        sim.add_gate(gate);
                    } else if (event.key.keysym.sym == SDLK_n) {
                        Not* gate = new Not(mouse_pos.x, mouse_pos.y);
                        sim.add_gate(gate);
                    } else if (event.key.keysym.sym == SDLK_i) {
                        Input* input = new Input(mouse_pos.x, mouse_pos.y);
                        sim.add_input(input);
                    }
                    break;
                default:
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(rend, 110, 110, 110, 255);   
        SDL_RenderClear(rend);
        
        sim.draw(rend);
        sim.simulate();
        
        SDL_RenderPresent(rend); 
    }

    
    //cleanup
    SDL_DestroyRenderer(rend); 
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
