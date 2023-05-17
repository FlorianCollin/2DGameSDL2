#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include "SDL_fonction.h"
#include "sdl_utils.h"
#include "menu.h"
#include "plateau.h"
#include "entity.h"
#include "bloc.h"
#include "player.h"
#include "config.h"
//#include "inventory.h"


/*

J'ai commencer ce projet en voulant rendre le tout très modulable et evolutif mais je choisi de fixer quelque paramètre clé.

SIZE ENTITY SPRITE PLAYER = 32 !


*/


// 2 MODE DE DEPLACEMENTS : MOOVE ET DEPLACE

// For testing !

#define MOOVE

#ifndef MOOVE 
#define DEPLACE
#endif

int main() {

    Config config;
    config.load("./config/config.txt");
    config.affiche();
    
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr, *back_texture = nullptr;

    if (fullInit(&window, &renderer, config.getSCREEN_WIDTH(), config.getSCREEN_HEIGHT())) {
        std::cerr << "Erreur lors de l'initialisation de SDL" << std::endl;
        destroyAll(window, renderer, texture);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Erreur lors de l'initialisation de TTF: " << TTF_GetError() << std::endl;
        destroyAll(window, renderer, texture, back_texture);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("./asset/font/font.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Erreur lors du chargement de la police: " << TTF_GetError() << std::endl;
        destroyAll(window, renderer, texture, back_texture);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    
    //la methode du fond sera modifier plus tard
    back_texture = loadImage("./asset/image/Sprite-0001.bmp", renderer);
    if (back_texture == nullptr){
        std::cerr << "loadImage Fail" << std::endl;
        destroyAll(window, renderer, texture, back_texture);
        SDL_Quit();
        return 1;
    }

    Bloc mur("./asset/image/Sprite-0002.bmp", renderer);

    Plateau plateau0(0, config.getBLOCK_SIZE(), config.getSCREEN_WIDTH(), config.getSCREEN_HEIGHT(), config.getGRID_WIDTH(), config.getGRID_HEIGHT());
    if (config.getBLOCK_SIZE() == 32) plateau0.map("./asset/map/map32.txt");
    else if (config.getBLOCK_SIZE() == 16) plateau0.map("./asset/map/map16.txt");
    //plateau0.edit(3, 3, 1);
    plateau0.affiche();


    Player player(0,0,config.getBLOCK_SIZE());
    player.load("./asset/image/AnimationSheet_Character.png", renderer);

    //Inventory inv("./image/inventory.bmp", renderer);

    float currentTime = SDL_GetTicks();
    float previousTime;
    float deltaTime;

    int mouseX, mouseY;
    SDL_Rect selectRect;
    selectRect.w = config.getBLOCK_SIZE();
    selectRect.h = config.getBLOCK_SIZE();

    showMenu(renderer);


    bool cursorInWindow = true;
    bool showStats = false;
    bool quit = false;

    bool edit1 = false;
    bool edit0 = false;

    bool noKeyPress = true;

    Direction currentDirection = NOT_MOOVING;
    Direction lastDirection = NOT_MOOVING;

    bool switch_animation = false;

    SDL_Event event;
    while(!quit) {
        previousTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - previousTime;
        fpsWait(deltaTime);
        while (SDL_PollEvent(&event)) {    
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                noKeyPress = false;
                switch (event.key.keysym.sym) {
                    case SDLK_d:
                        lastDirection = currentDirection;
                        currentDirection = RIGHT;
                        player.moove(currentDirection, plateau0);
                        break;
                    case SDLK_q:
                        lastDirection = currentDirection;
                        currentDirection = LEFT;
                        player.moove(currentDirection, plateau0);                        
                        break;
                    case SDLK_s:
                        lastDirection = currentDirection;
                        currentDirection = DOWN;
                        player.moove(currentDirection, plateau0);                        
                        break;
                    case SDLK_z:
                        lastDirection = currentDirection;
                        currentDirection = UP;
                        player.moove(currentDirection, plateau0);                        
                        break;

                    case SDLK_i:
                        showStats = true;
                        break;

                    case SDLK_m:
                        edit1 = true;
                        break;

                    case SDLK_n:
                        edit0 = true;
                        break;

                    case SDLK_l:
                        plateau0.save();
                        break;

                }
            }

            else if (event.type == SDL_KEYUP) {
                noKeyPress = true;
                if (switch_animation == false) switch_animation = true;
                lastDirection = currentDirection;
                currentDirection = NOT_MOOVING;

                switch (event.key.keysym.sym) {
                    case SDLK_i:
                        showStats = false;
                        break;
                }
            }

            else if (event.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&mouseX, &mouseY);
                selectRect.x = (mouseX / config.getBLOCK_SIZE()) * config.getBLOCK_SIZE();
                selectRect.y = (mouseY / config.getBLOCK_SIZE()) * config.getBLOCK_SIZE();
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_ENTER) {
                    cursorInWindow = true;
                }
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
                    cursorInWindow = false;
                }
            }
            else {
                lastDirection == currentDirection;

            }

            
        }
        if (edit1) {
            std::cout << selectRect.x/config.getBLOCK_SIZE() << " " << selectRect.y/config.getBLOCK_SIZE() << std::endl;
            plateau0.edit(selectRect.x/config.getBLOCK_SIZE(), selectRect.y/config.getBLOCK_SIZE(), 1);
            edit1 = false;
        }
        else if (edit0) {
            std::cout << selectRect.x/config.getBLOCK_SIZE() << " " << selectRect.y/config.getBLOCK_SIZE() << std::endl;
            plateau0.edit(selectRect.x/config.getBLOCK_SIZE(), selectRect.y/config.getBLOCK_SIZE(), 0);
            edit0 = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        texturePaste(renderer, texture, back_texture);
        plateau0.draw(renderer, texture, mur);


        if (currentDirection != NOT_MOOVING) {
            if (lastDirection == NOT_MOOVING) player.setAnimation("moove");
            player.setDirection(currentDirection);
            player.updateAnimation((float)(1000.0/30.0));
            player.drawAnimation(renderer);
        }
        else {
            //void Player::setAnimation(const std::string& name) {
            if (switch_animation) {
                player.setAnimation("basic");
                switch_animation = false;
                player.setDirection(currentDirection);

            }
            player.updateAnimation((float)(1000.0/30.0));
            player.drawAnimation(renderer);
        }
        SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE);
         if (cursorInWindow) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &selectRect);
        }
        player.displayStats(renderer, font, showStats, config.getSCREEN_WIDTH(), config.getSCREEN_HEIGHT());
        SDL_RenderPresent(renderer);

    }
    destroyAll(window, renderer, texture, back_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
