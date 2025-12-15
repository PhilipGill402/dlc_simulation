#include "main.h"

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::perror("error initializeing SDL: ");
        std::cout << SDL_GetError() << "\n";
    }
        
    //width and height constants
    constexpr int width = 800;
    constexpr int height = 600;

    SDL_Window* win = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); 
    
    And gate = And(true, true, 400, 300);

    //sets backround to grey
    SDL_SetRenderDrawColor(rend, 110, 110, 110, 255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    bool close = false;
    while (!close) {
        SDL_Event event; 

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = true;
                    break;
                default:
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(rend, 110, 110, 110, 255);   
        SDL_RenderClear(rend);

        gate.draw(rend);

        SDL_RenderPresent(rend); 
    }

    
    //cleanup
    SDL_DestroyRenderer(rend); 
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
