#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>
#include <fstream>

typedef struct item ITEM;


struct item {
	SDL_Texture *texture;
	std::string name;
	std::string description;
};
#endif 