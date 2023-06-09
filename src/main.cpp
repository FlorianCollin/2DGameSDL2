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



/*
TODO : 

    - Changer la manière dont les caractéristiques du jeu sont fournies
     --> utiliser une structure config plutot qu'une classe la mettre en variable dans chaque instance d'une classe

    - Revoir la manière dont sont gérer les classes, mieux subdiviser les classes
     --> une classe item 
        --> une classe entity 
            --> etc


*/

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
    plateau0.map("./asset/map/map.txt"); // je decide finalement que l'on aurra tjr une grille de 20 par 15 
    // sachant que l'on a deux mode de fonctionnement possible 32 et 64
    plateau0.affiche();


    Player player(config.getSCREEN_WIDTH()/2 - config.getBLOCK_SIZE(),config.getSCREEN_HEIGHT() - 2*config.getBLOCK_SIZE(),config.getBLOCK_SIZE());
    player.load("./asset/image/AnimationSheet_Character.png", renderer);

    //Inventory inv("./image/inventory.bmp", renderer);

    float currentTime = SDL_GetTicks();
    float previousTime;
    float deltaTime;

    int mouseX, mouseY;
    SDL_Rect selectRect;
    selectRect.w = config.getBLOCK_SIZE();
    selectRect.h = config.getBLOCK_SIZE();

    showMenu(renderer, config);


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
                        player.setDirection(RIGHT);
                        break;
                    case SDLK_q:
                        player.setDirection(LEFT);
                        break;
                    case SDLK_s:
                        player.setDirection(DOWN);
                        break;
                    case SDLK_z:
                      player.setDirection(UP);
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
                    case SDLK_SPACE:
                        if (player.getIsJumping() == 0) {
                            player.resetSpeedY();
                            player.setIsJumping();
                        }
                        break;

                }
            }

            else if (event.type == SDL_KEYUP) {
                noKeyPress = true;
                if (switch_animation == false) switch_animation = true;
                player.setDirection(NOT_MOOVING);
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

            
        } // fin de la boucle Pollevent

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

        player.update(plateau0, config);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        texturePaste(renderer, texture, back_texture);
        plateau0.draw(renderer, texture, mur);


        if (player.getDirection() != NOT_MOOVING) {
            if (player.getLastDirection() == NOT_MOOVING) player.setAnimation("moove");
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

    // Free section !

    destroyAll(window, renderer, texture, back_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

