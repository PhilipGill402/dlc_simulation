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

    while (!close) {
        SDL_Event event; 
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_pos = { event.motion.x, event.motion.y };
                    if (mouse_button_down && selected_gate && !dragging) {
                        int dx = mouse_pos.x - down_pos.x;
                        int dy = mouse_pos.y - down_pos.y;
                        std::cout << dx << ", " << dy << "\n";
                        if (dx*dx + dy*dy >= drag_threshold*drag_threshold) {
                            dragging = true; 
                            selected_gate->x = mouse_pos.x - offset_pos.x;
                            selected_gate->y = mouse_pos.y - offset_pos.y;
                        }

                        
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (mouse_button_down && event.button.button == SDL_BUTTON_LEFT) {
                        mouse_button_down = false;
                        selected_gate = nullptr;
                    } else if (!dragging) {
                        for (Input* input : sim.inputs) {
                            if (point_in_circle(mouse_pos, input->x, input->y, 20)) {
                                input->toggle(); 
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    if (!mouse_button_down && event.button.button == SDL_BUTTON_LEFT) {
                        mouse_button_down = true;
                         
                        down_pos = { event.motion.x, event.motion.y };

                        for (Gate* gate : sim.gates) {
                            if (SDL_PointInRect(&mouse_pos, &gate->rect)) {
                                selected_gate = gate;

                                offset_pos.x = mouse_pos.x - gate->x;
                                offset_pos.y = mouse_pos.y - gate->y;
                                break;
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
