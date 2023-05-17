#ifndef SDL_FONCTION_H
#define SDL_FONCTION_H

#include<SDL2/SDL.h>

// j'ajoute égalemet ici les fonction utilitaire d'aure mathématique 


int fullInit(SDL_Window **window, SDL_Renderer **renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT);
//initialise SDL video, window et renderer.

int fpsWait(float deltaTime, float fps = 30.0);
// Fournir en input le nombre de ms écoulé et le nombre de fps souhaité et la fonction ajoutera un delay adapté.

int initTexture(SDL_Renderer *renderer, SDL_Texture *texture, int w, int h);

SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer);

int drawImage(SDL_Texture *image_texture, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rectCut, SDL_Rect *rectPos, int angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);

int texturePaste(SDL_Renderer *renderer, SDL_Texture *texture_cible, SDL_Texture *texture_tmp);

SDL_Rect giveRect(int k);

void afficheRect(SDL_Rect rect);

// fonction utile

int giveIndice(int x, int y); // Ne pas utiliser à modifier

int getCaseIndex(SDL_Point point);


#endif