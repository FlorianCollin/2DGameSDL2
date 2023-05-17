/*
Cette class est la classe mère des futur entité mais elle peut être utilisé tels quelle pour certaine entité élémentaire.
Elle contient les informations de position, de taille (SDL_Rect) et le texture ainsi que le nombre d'entité total qui est actuellement disponible sur le jeu.
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

#include "plateau.h"
#include "SDL_fonction.h"
#include "enum.h"

#define HIT_BOX

class Entity {
protected:
    SDL_Rect rect;
    SDL_Texture *texture;
    float moove_speed;
    static int nb_entity;

public:
    Entity(int x = 0, int y = 0, int ENTITY_SIZE = 32, float moove_speed_ = 200);
    ~Entity(); 
    //Pour Destroy la texture
    void load(const char* path, SDL_Renderer *renderer); 
    // Il faut "load()" juste après la création d'une entity 
    // Si aucune image a été charger on desinera un rectangle de couleur rouge par défault.
    void affiche(); //affiche les caractéristique de l'entity (rect.

    // 2 méthode de déplacement : deplace qui déplace dans la grille et moove qui déplace naturellement
    virtual void deplace(int dx, int dy,const Plateau &plat); 
    void moove(int dx, int dy, Plateau &plat); //ici je fais le choix de fixer le nombre de fps à 30
    void moove(Direction direction, Plateau &plat); // version on l'on fournit uniquement la direction
    virtual int draw(SDL_Renderer* renderer, SDL_Texture *textureCible); 
    // "draw()", affiche l'entity sur le rendu, si aucune texture chargé alors rectangle rouge.

    // j'ajoute ici des fonctions supplémentaire d'acces (on fournit par copie)

    SDL_Rect getRect();
    float getSpeed();
    int getNumberEntity();

    // Fonction de modification direct

    // a ecrire ci besoin
    
};

#endif
