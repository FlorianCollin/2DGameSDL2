#ifndef BLOC_H
#define BLOC_H
#include "SDL2/SDL.h"
#include "SDL_fonction.h"
#include <iostream>

class Plateau;

class Bloc{
private :
	SDL_Texture *texture;
public :

	Bloc(const char *path, SDL_Renderer *renderer);
	~Bloc();

friend class Plateau;

};

#endif