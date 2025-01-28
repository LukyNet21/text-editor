
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <string.h>
// Example change
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Could not open file %s!", argv[1]);
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fclose(file);
        return -1;
    }

    if (TTF_Init() == -1) {
        SDL_Quit();
        fclose(file);
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("File Explorer",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        TTF_Quit();
        SDL_Quit();
        fclose(file);
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        fclose(file);
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("../assets/Arial.ttf", 24);
    if (font == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        fclose(file);
        return -1;
    }

    fseek(file, 0, SEEK_SET);

    int quit = 0;
    SDL_Event e;

    char line[1024];
    SDL_Color textColor = {255, 255, 255};
    int yOffset = 0;

    char fileContent[10240];
    int fileIndex = 0;

    int cursor_status = 1;
    Uint32 lastSwitchTime = 0;
    int cursorCharX = 0;

    while (fgets(line, sizeof(line), file)) {
        for (int i = 0; line[i] != '\0'; ++i) {
            fileContent[fileIndex++] = line[i];
        }
    }
    fileContent[fileIndex] = '\0';

    char* lines[100];
    int lineCount = 0;
    char* token = strtok(fileContent, "\n");
    while (token != NULL) {
        lines[lineCount++] = token;
        token = strtok(NULL, "\n");
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            switch (e.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEMOTION:
                    // printf("%d, %d\n", e.motion.x, e.motion.y);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            printf("UP!\n");
                            break;
                        case SDLK_DOWN:
                            printf("DOWN!\n");
                            break;
                        case SDLK_RIGHT:
                            if (cursorCharX < fileIndex) {
                                cursorCharX++;
                            }
                            break;
                        case SDLK_LEFT:
                            if (cursorCharX > 0) {
                                cursorCharX--;
                            }
                            break;

                    }

                    break;
            }
        }


        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
        SDL_RenderClear(renderer);

        yOffset = 0;
        int contentIndex = 0;

        while (contentIndex < fileIndex) {
            int lineEnd = contentIndex;
            while (fileContent[lineEnd] != '\n' && fileContent[lineEnd] != '\0') {
                lineEnd++;
            }

            char currentLine[1024];
            int lineLength = lineEnd - contentIndex;
            strncpy(currentLine, &fileContent[contentIndex], lineLength);
            currentLine[lineLength] = '\0';

            SDL_Surface* textSurface = TTF_RenderText_Solid(font, currentLine, textColor);
            if (textSurface == NULL) {
                break;
            }
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            SDL_Rect renderQuad = { 50, 50 + yOffset, textSurface->w, textSurface->h };

            SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
            SDL_DestroyTexture(textTexture);

            yOffset += textSurface->h;
            contentIndex = lineEnd + 1;

            if (yOffset <= textSurface->h) {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastSwitchTime >= 250) {
                    cursor_status = 1 - cursor_status;
                    lastSwitchTime = currentTime;
                }

                int cursorLineX = 50;
                for (int i = 0; i < cursorCharX; ++i) {
                    char currentChar[2] = {currentLine[i], '\0'};
                    int charWidth;
                    TTF_SizeText(font, currentChar, &charWidth, NULL);
                    cursorLineX += charWidth;
                }

                if (cursor_status) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_Rect cursorRect = {cursorLineX, 50, 1, textSurface->h};
                    SDL_RenderFillRect(renderer, &cursorRect);
                }
            }
            SDL_FreeSurface(textSurface);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }


    fclose(file);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
