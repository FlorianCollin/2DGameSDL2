#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <iostream>

// Fonction de base pour terminer la récursion
void destroyAll(SDL_Window* window, SDL_Renderer* renderer) {
    if (nullptr != renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (nullptr != window) {
        SDL_DestroyWindow(window);
    }

    std::cout << "destroyAll succed!\n";
}

// Fonction template pour détruire les textures
template<typename T, typename... Ts>
void destroyAll(SDL_Window* window, SDL_Renderer* renderer, T* first, Ts*... rest) {
    if (nullptr != first) {
        SDL_DestroyTexture(first);
    }
    destroyAll(window, renderer, rest...);
}

#endif // SDL_UTILS_H
