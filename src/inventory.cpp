#include "inventory.h"

Inventory::Inventory(const char * path, SDL_Renderer *renderer){
	texture = loadImage("./bmp/inventory.bmp", renderer);
}

Inventory::~Inventory(){
	SDL_DestroyTexture(texture);
}

void Inventory::affiche(SDL_Renderer* renderer) {	
    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 480;


    SDL_Rect full_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    
    bool inventory_active = true;
    while (inventory_active) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            inventory_active = false;
            SDL_Quit();
            exit(0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
  			// à ajouter
        }
        else if (event.type == SDL_KEYDOWN) {
        	switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                	inventory_active = false;
                	break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, &full_screen);
        SDL_RenderPresent(renderer);
    	}
	}
}

void Inventory::add_item(ITEM new_item) {
	if(nb_item < 5) {
		item_tab[nb_item] = new_item;
		nb_item++;
	}
	else {
		std::cout << "L'inventaire est plein." << std::endl;
	}
}