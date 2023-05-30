/*
	On retrouvera ici tout ce qui est menu, affichage de texte, ..., qui ne ce réfère pas un une entité particulière mais au jeu en général
*/
#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>

#include "config.h"


void showMenu(SDL_Renderer* renderer, Config &config);
bool isInside(SDL_Rect rect, int x, int y);

#endif