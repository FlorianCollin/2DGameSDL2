// le code fonctionne nativement avec tout en 32 x 32 mais des fonctions ont été rajouter pour rendre compatible le fonctionnemnt 64 x 64 sans surcharger les fonctions déjà établie
#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <map>


struct Animation {
    int startFrame;
    int numFrames; // Nombre de frame dans l'animation 
    int lineFrame; // Numéro de la ligne ou ce trouve les sprites de l'animation
    float frameDuration; // Duréé qui sépare chaque changement d'image
};

int check(SDL_Rect rect_tmp, Plateau &plat);


class Player : public Entity {
protected:
    int hp;
    int exp; 
    int level;
    int attack_strength;
    int speed;
    Direction currentDirection;
    Direction lastDirection;

    // gestion de la gravité

    bool isJumping;
    int speedY;
    int gravity;
    
    // pour les animations du player // attention chaque sprite doit STRICTEMENT faire 32 par 32 pixels

    int frameWidth; // Largeur d'une seule image dans la feuille d'animation
    int frameHeight; // Hauteur d'une seule image dans la feuille d'animation
    int currentFrame; // Index de l'image actuelle (démarre à 0)
    float frameTime; // Temps écoulé depuis le dernier changement d'image
    

    std::map<std::string, Animation> animations;
    /*
    std::map<std::string, Animation> animations; est une déclaration de variable appelée animations. animations est une instance de la classe std::map provenant de la bibliothèque standard C++ (d'où le préfixe std::). La classe std::map est un conteneur associatif qui stocke des paires clé-valeur, où chaque clé est unique.
    Dans ce cas, les clés sont des chaînes de caractères (std::string) et les valeurs sont des instances de la structure Animation. Ainsi, animations est un conteneur qui peut stocker des objets Animation et les associer à des noms sous forme de chaînes de caractères.
    */
    Animation* currentAnimation;


public:

    Player(int x = 0, int y = 0, int ENTITY_SIZE = 32, float moove_speed = 500, int hp = 100, int exp = 0, int level = 1, int attack_strength = 10, int speed = 1);
    void gainExp(int exp_points);
    void levelUp();
    void takeDamage(int damage); 
    bool isAlive(); 
    // surdefinition de moove à faire
    void deplace(int dx, int dy, const Plateau &plat) override; // je n'utilise pas cette fonction dans mon code final
    int draw(SDL_Renderer* renderer, SDL_Texture *textureCible) override;

    void displayStats(SDL_Renderer* renderer, TTF_Font* font, bool showStats, int SCREEN_WIDTH, int SCREEN_HEIGHT);

    // Pour les animations

    void addAnimation(const std::string& name, int startFrame, int numFrames, int lineFrame, float frameDuration);
    void updateAnimation(float deltaTime);
    void drawAnimation(SDL_Renderer* renderer);
    void setAnimation(const std::string& name);

    // Direction

    Direction getDirection();
    Direction getLastDirection();
    Direction setDirection(Direction newDirection); // cette fonctin set une nouvvelle direction et retourne l'ancienne

    // Saut

    bool getJumpingState();
    bool setJumpingState(bool state);

    // Deplacement avec gravité

    int update(Plateau &plat, Config &config); //limitation 30 fps
    void setIsJumping();
    bool getIsJumping();
    void resetSpeedY();
};

#endif