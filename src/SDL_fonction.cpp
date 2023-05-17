#include"SDL_fonction.h"
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<iostream>


int fullInit(SDL_Window **window, SDL_Renderer **renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1; 
    }
    if(0 != SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, window, renderer)){
        fprintf(stderr,"Erreur SDL_CreateWindowAndRenderer : %s",SDL_GetError());
        return -1; 
    }
    return 0; 
}

int fpsWait(float deltaTime, float fps) {
    float waitTime;
    waitTime = (1.0 / fps) * 1000 - deltaTime;
    if (waitTime * 1000.0f > 0) {
        SDL_Delay(static_cast<int>(waitTime)); 
    }
    return 0;
}

int initTexture(SDL_Renderer *renderer, SDL_Texture *texture, int w, int h) {  
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if(nullptr == texture){
        fprintf(stderr, "ERREUR SDL_CreateTexture: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

// SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer) {
//     SDL_Surface *tmp = nullptr; // on va y load l'image
//     SDL_Texture *texture = nullptr;
//     tmp = SDL_LoadBMP(path);
//     if (tmp == nullptr){
//         fprintf(stderr," ERREUR SDL_LoadBMP: %s", SDL_GetError());
//         return nullptr;
//     }
//     texture = SDL_CreateTextureFromSurface(renderer, tmp);
//     SDL_FreeSurface(tmp);
//     if (nullptr == texture){
//         fprintf(stderr, "ERREUR SDL_CreateTextureFromSurface : %s", SDL_GetError());
//         return nullptr;
//     }
//     return texture; 
// }

SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path); // On utilise ici la fonction IMG_Load de SDL_image.
    if (loadedSurface == nullptr) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    }
    else {
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    }
    std::cout << "image load : " << path << std::endl;
    return texture;
}

int drawImage(SDL_Texture *image_texture, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rectCut, SDL_Rect *rectPos, int angle, const SDL_RendererFlip flip) { 
    SDL_SetRenderTarget(renderer, texture); //la cible du rendu devient texture
    // /https://wiki.libsdl.org/SDL_RenderCopyEx
    SDL_RenderCopyEx(renderer, image_texture, rectCut, rectPos, angle, nullptr, flip);
    SDL_SetRenderTarget(renderer, nullptr);
    return 0;
}



int texturePaste(SDL_Renderer *renderer, SDL_Texture *texture_cible, SDL_Texture *texture_tmp) {
    SDL_SetRenderTarget(renderer, texture_cible);
    SDL_RenderCopy(renderer, texture_tmp, nullptr, nullptr);
    SDL_SetRenderTarget(renderer, nullptr);
    return 0;
}



SDL_Rect giveRect(int k) {
    SDL_Rect rect;
    int size = 32; // repétitif
    rect.w = size;
    rect.h = size;
    int nb_lignes = 15;
    int nb_colonnes = 20;
    int j = k / nb_colonnes;
    int i = k % nb_colonnes;
    rect.x = i * size;
    rect.y = j * size;
    return rect;

}

SDL_Rect giveRect64(int k) {
    SDL_Rect rect;
    int size = 64; // repétitif
    rect.w = size;
    rect.h = size;
    int nb_lignes = 15;
    int nb_colonnes = 20;
    int j = k / nb_colonnes;
    int i = k % nb_colonnes;
    rect.x = i * size;
    rect.y = j * size;
    return rect;

}



void afficheRect(SDL_Rect rect) {
    printf("rect : %d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
}

int giveIndice(int x, int y) {
    int nb_colonnes = 20; // repétitif
    int indice = 0;
    int size = 32;
    int i, j; // on pase par le calcul de i et j qui sont le numéro de la ligne de la case et de la collone
    // En effet le point de coordonnée (x, y) ce trouve dans la case numéro "num" qui est elle même aux coordonnées (i,j) dans le plateau

    i = x / size; // quotient
    j = y / size;
    //printf("giveIndice : x,y -> i,j : (%d,%d)->(%d %d)", x, y, i, j);
    indice = i * nb_colonnes + j;

    return indice;
}

int getCaseIndex(SDL_Point point) {
    // La largeur et la hauteur du plateau en nombre de cases
    const int width = 640 / 32;
    const int height = 480 / 32;

    // Vérification que le point est bien dans les limites du plateau
    if (point.x < 0 || point.x >= 640 || point.y < 0 || point.y >= 480) {
        // Le point est en dehors du plateau
        return -1;
    }

    // Conversion des coordonnées du point en index du tableau
    int index = (point.y / 32) * width + (point.x / 32);

    // Vérification que l'index est bien dans les limites du tableau
    if (index < 0 || index >= width * height) {
        // L'index est en dehors du tableau
        return -1;
    }

    return index;
}


// Pour faire fonctionner en taille 64 et non 32

int giveIndice64(int x, int y) {
    int nb_colonnes = 20; // repétitif
    int indice = 0;
    int size = 64;
    int i, j; // on pase par le calcul de i et j qui sont le numéro de la ligne de la case et de la collone
    // En effet le point de coordonnée (x, y) ce trouve dans la case numéro "num" qui est elle même aux coordonnées (i,j) dans le plateau

    i = x / size; // quotient
    j = y / size;
    //printf("giveIndice : x,y -> i,j : (%d,%d)->(%d %d)", x, y, i, j);
    indice = i * nb_colonnes + j;

    return indice;
}

int getCaseIndex64(SDL_Point point) {
    // La largeur et la hauteur du plateau en nombre de cases
    const int width = 20;
    const int height = 15;

    // Vérification que le point est bien dans les limites du plateau
    if (point.x < 0 || point.x >= 20*64 || point.y < 0 || point.y >= 15*64) {
        // Le point est en dehors du plateau
        return -1;
    }

    // Conversion des coordonnées du point en index du tableau
    int index = (point.y / 64) * width + (point.x / 64);

    // Vérification que l'index est bien dans les limites du tableau
    if (index < 0 || index >= width * height) {
        // L'index est en dehors du tableau
        return -1;
    }

    return index;
}
