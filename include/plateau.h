#ifndef PLAT_H
#define PLAT_H

#include <iostream>
#include <string.h>
#include <fstream>
#include <SDL2/SDL.h>
#include "bloc.h"

class Entity;

class Plateau{
private :
	int *tab;
	int num;

	int BLOC_SIZE;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int GRID_WIDTH;
	int GRID_HEIGHT;

	static int nb_plat;
	
public :
	Plateau(int n, int bs,int sw, int sh,int gw,int gh);
	~Plateau();
	void map(const std::string &nomFichier);
	void save();
	void edit(int k, int value);
	void edit(int i, int j, int value);
	void draw(SDL_Renderer *renderer, SDL_Texture *texture, const Bloc &mur);
	void affiche();
	void affiche(int i, int j);
	void affiche(int k);
	int get(int k);

	friend class Entity;
};



#endif