#include "player.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>




Player::Player(int x, int y, int ENTITY_SIZE, float moove_speed, int hp, int exp, int level, int attack_strength, int speed)
    : Entity(x, y, ENTITY_SIZE, moove_speed), hp(hp), exp(exp), level(level), attack_strength(attack_strength), speed(speed) {
    frameWidth = ENTITY_SIZE;
    frameHeight = ENTITY_SIZE;
    // Ici on place les animation du joueur 
    addAnimation("basic", 0, 2, 0, 500);
    addAnimation("moove", 0, 8, 3, 250);
    // L'animation de démarage sera la basic 
    currentAnimation = &animations["basic"];
    currentFrame = 0;
    frameTime = 0;
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
    indice0 = getCaseIndex(P0);
    indice1 = getCaseIndex(P1);
    indice2 = getCaseIndex(P2);
    indice3 = getCaseIndex(P3);
    SDL_Rect rect0, rect1, rect2, rect3;
    rect0 = giveRect(indice0);
    rect1 = giveRect(indice1);
    rect2 = giveRect(indice2);
    rect3 = giveRect(indice3);
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

// Direction getDirection();
// Direction setDirection(Direction newDirection);

Direction Player::getDirection() {
    return currentDirection;
}

Direction Player::setDirection(Direction newDirection) {
    lastDirection = currentDirection;
    currentDirection = newDirection;
    return lastDirection;
}