#ifndef INV_H
#define INV_H
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include "item.h"
#include "SDL_fonction.h"

class Inventory{
	private :
		const char* path;
		SDL_Texture *texture;
		ITEM * item_tab;
		SDL_Rect * rect_tab;
		int nb_item;
	public :
		Inventory(const char *path, SDL_Renderer *renderer);
		~Inventory(); // Destroy la texture
		void affiche(SDL_Renderer *renderer);
		void add_item(ITEM new_item);

};
#endif