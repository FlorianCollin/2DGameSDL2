#include "menu.h"
#include "SDL_fonction.h"

bool isInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w) && (y >= rect.y && y <= rect.y + rect.h);
}

void showMenu(SDL_Renderer* renderer, Config &config) {
    int SCREEN_WIDTH = config.getSCREEN_WIDTH();
    int SCREEN_HEIGHT = config.getSCREEN_HEIGHT();
    SDL_Texture* play_button_texture = loadImage("./asset/image/play_button.bmp", renderer);
    if (play_button_texture == nullptr) {
        printf("Failed to load play_button.bmp\n");
    }
    
    SDL_Rect play_button_rect;
    play_button_rect.w = 200;
    play_button_rect.h = 80;
    play_button_rect.x = (SCREEN_WIDTH - play_button_rect.w) / 2;
    play_button_rect.y = (SCREEN_HEIGHT - play_button_rect.h) / 2;

    bool menu_active = true;
    while (menu_active) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            menu_active = false;
            SDL_Quit();
            exit(0);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (isInside(play_button_rect, x, y)) {
                menu_active = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, play_button_texture, nullptr, &play_button_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(play_button_texture);
}
