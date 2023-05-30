#include "player.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>




Player::Player(int x, int y, int ENTITY_SIZE, float moove_speed, int hp, int exp, int level, int attack_strength, int speed)
    : Entity(x, y, ENTITY_SIZE, moove_speed), hp(hp), exp(exp), level(level), attack_strength(attack_strength), speed(speed) {
    frameWidth = 32;
    frameHeight = 32;
    // Ici on place les animation du joueur 
    addAnimation("basic", 0, 2, 0, 500);
    addAnimation("moove", 0, 8, 3, 250);
    // L'animation de démarage sera la basic 
    currentAnimation = &animations["basic"];
    currentFrame = 0;
    frameTime = 0;
    speedY = -1000;
    gravity = 200;
}

void Player::gainExp(int exp_points) {
    exp += exp_points;
    if (exp >= 100 * level) {
        levelUp();
    }
}

void Player::levelUp() {
    exp -= 100 * level;
    level++;
    attack_strength += 5;
    hp += 20;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Player::isAlive() {
    return hp > 0;
}

// redefinition des methodes virtuelles

void Player::deplace(int dx, int dy, const Plateau &plat) {
    for (int i = 0; i < speed; i++) {
        Entity::deplace(dx, dy, plat);
    }
}

int Player::draw(SDL_Renderer* renderer, SDL_Texture *textureCible) {
    Entity::draw(renderer, textureCible);
    return 0;
}



void Player::displayStats(SDL_Renderer* renderer, TTF_Font* font, bool showStats, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    if (showStats) {
        std::stringstream ss;
        ss << "HP: " << hp << "\nExp: " << exp << "\nLevel: " << level << "\nAttack: " << attack_strength << "\nSpeed: " << speed;

        std::string stats = ss.str();
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, stats.c_str(), color, 300);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect destRect;
        destRect.x = (SCREEN_WIDTH - surface->w) / 2;
        destRect.y = (SCREEN_HEIGHT - surface->h) / 2;
        destRect.w = surface->w;
        destRect.h = surface->h;

        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

// pour les animations

void Player::addAnimation(const std::string& name, int startFrame, int numFrames, int lineFrame, float frameDuration) {
    Animation tempAnimation = {startFrame, numFrames, lineFrame, frameDuration};
    animations[name] = tempAnimation;
}


void Player::updateAnimation(float deltaTime) {
    frameTime += deltaTime;
    if (frameTime >= currentAnimation->frameDuration) {
        currentFrame = (currentFrame + 1) % currentAnimation->numFrames;
        frameTime = 0;
    }
}

void Player::drawAnimation(SDL_Renderer* renderer) {

    SDL_Rect srcRect = {(currentAnimation->startFrame + currentFrame) * frameWidth, (currentAnimation->lineFrame) * frameWidth, frameWidth, frameHeight};
    if (currentDirection == LEFT || (lastDirection == LEFT && currentDirection == NOT_MOOVING)) {
        SDL_RenderCopyEx(renderer, texture, &srcRect, &rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, texture, &srcRect, &rect);
    }
    
    #ifdef HIT_BOX
    // On desine tout les carrés ou il y à un des coins de l'entity joueur 
    SDL_Point P0, P1, P2, P3;
    P0.x = rect.x;          
    P0.y = rect.y;
    P1.x = rect.x + rect.w;          
    P1.y = rect.y;
    P2.x = rect.x + rect.w;          
    P2.y = rect.y + rect.h;
    P3.x = rect.x;          
    P3.y = rect.y + rect.h;
    int indice0, indice1, indice2, indice3;
    SDL_Rect rect0, rect1, rect2, rect3;


    if (rect.w == 32) {
        indice0 = getCaseIndex(P0);
        indice1 = getCaseIndex(P1);
        indice2 = getCaseIndex(P2);
        indice3 = getCaseIndex(P3);
        rect0 = giveRect(indice0);
        rect1 = giveRect(indice1);
        rect2 = giveRect(indice2);
        rect3 = giveRect(indice3);
        
    }
    else if (rect.w == 64) {
        indice0 = getCaseIndex64(P0);
        indice1 = getCaseIndex64(P1);
        indice2 = getCaseIndex64(P2);
        indice3 = getCaseIndex64(P3);
        rect0 = giveRect64(indice0);
        rect1 = giveRect64(indice1);
        rect2 = giveRect64(indice2);
        rect3 = giveRect64(indice3);
    }
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect0);
    SDL_RenderDrawRect(renderer, &rect1);
    SDL_RenderDrawRect(renderer, &rect2);
    SDL_RenderDrawRect(renderer, &rect3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    #endif
}

void Player::setAnimation(const std::string& name) {
    currentAnimation = &animations[name];
    currentFrame = 0;
    frameTime = 0;
}


Direction Player::getDirection() {
    return currentDirection;
}

Direction Player::getLastDirection() {
    return lastDirection;
}

Direction Player::setDirection(Direction newDirection) {
    lastDirection = currentDirection;
    currentDirection = newDirection;
    return lastDirection;
}


bool Player::getJumpingState() {
    return isJumping;
}

bool Player::setJumpingState(bool state) {
    bool lastJumpingState = isJumping;
    isJumping = state;
    return lastJumpingState;
}

// la fonction update géneralise les fonctions de deplacement et ajoute le jump 

int Player::update(Plateau &plat, Config &config) {

    float deltaTime = 1.0/30.0; // Ici on considère que l‡e jeu tourne à 30 fps fixe   // NOTE : cette pratique est dangeureuse !!
    int SCREEN_WIDTH = config.getSCREEN_WIDTH();
    int SCREEN_HEIGHT = config.getSCREEN_HEIGHT();
    int BLOCK_SIZE = rect.w;

    if (isJumping) { 
        rect.y += speedY * deltaTime;
    }

    SDL_Rect mur_rect;
    mur_rect.w = BLOCK_SIZE;
    mur_rect.h = BLOCK_SIZE;

    int nb_lignes = SCREEN_HEIGHT/BLOCK_SIZE;
    int nb_colonnes = SCREEN_WIDTH/BLOCK_SIZE;

    bool intersection = 0; // si intersection = 1 alors il y à intersectoin !

    SDL_Rect rect_tmp = rect;

    if (isJumping) { 
        rect_tmp.y += speedY * deltaTime;
    }

    // on moove un player virtuelle
    switch (currentDirection)
    {
    case RIGHT:
        rect_tmp.x += moove_speed*deltaTime;
        break;
    case LEFT:
        rect_tmp.x -= moove_speed*deltaTime;
        break;
    case  UP:
        rect_tmp.y -=  moove_speed*deltaTime; 
        break;
    case DOWN:
        rect_tmp.y += moove_speed*deltaTime; 
    default:
        break;
    }

    intersection = check(rect_tmp, plat);


    if (intersection == 0){
        afficheRect(rect_tmp);
        speedY += gravity * deltaTime;
        rect = rect_tmp;
    }
    else {
        if (isJumping == 1) {
            speedY = 0;
            isJumping = false;
        }
    }

    // La suite permet de refermet le bord gauche sur droit et le bord haut sur le bas

    if (rect.x < 0) {
        rect.x = SCREEN_WIDTH - BLOCK_SIZE;
    } else if (rect.x > SCREEN_WIDTH - BLOCK_SIZE) {
        rect.x = 0;
    }
    
    if (rect.y < 0) {
        rect.y = SCREEN_HEIGHT - BLOCK_SIZE;
    } else if (rect.y > SCREEN_HEIGHT - BLOCK_SIZE) {
        rect.y = 0;
    }


    return 1;
}

int check(SDL_Rect rect_tmp, Plateau &plat) {
    int intersection = 0;
    
    int w = rect_tmp.w; int h = rect_tmp.h;

    // Test de collision

    SDL_Point P0, P1, P2, P3;
    int indice0, indice1, indice2, indice3;
    P0.x = rect_tmp.x;          
    P0.y = rect_tmp.y;
    P1.x = rect_tmp.x + w;          
    P1.y = rect_tmp.y;
    P2.x = rect_tmp.x + w;          
    P2.y = rect_tmp.y + h;
    P3.x = rect_tmp.x;          
    P3.y = rect_tmp.y + h;

    if (rect_tmp.w == 32) {
        indice0 = getCaseIndex(P0);
        indice1 = getCaseIndex(P1);
        indice2 = getCaseIndex(P2);
        indice3 = getCaseIndex(P3);
    }
    else if (rect_tmp.w == 64) {
        indice0 = getCaseIndex64(P0);
        indice1 = getCaseIndex64(P1);
        indice2 = getCaseIndex64(P2);
        indice3 = getCaseIndex64(P3);
    }
    else printf("error size during the update\n");
    

    if (plat.get(indice0) == 1) {
        intersection = 1;
    } else if (plat.get(indice1) == 1) {
        intersection = 1;
    } else if (plat.get(indice2) == 1) {
        intersection = 1;
    } else if (plat.get(indice3) == 1) {
        intersection = 1;
    }

    return intersection;
}

void Player::setIsJumping() {
    isJumping = 1;
}

bool Player::getIsJumping() {
    return isJumping;
}

void Player::resetSpeedY() {
    speedY = -100;
}