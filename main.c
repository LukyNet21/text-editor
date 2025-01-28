#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("File explorer", 
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set the draw color (RGB)
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Blue color

    // Main loop flag
    int quit = 0;
    SDL_Event e;

    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            // Check if the user clicked the window close button
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen with the draw color
        SDL_RenderClear(renderer);

        // Present the rendered content to the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up and close the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}