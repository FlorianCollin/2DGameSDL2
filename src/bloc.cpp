#include "bloc.h"


Bloc::Bloc(const char* path, SDL_Renderer *renderer){
    texture = loadImage(path, renderer);
}
Bloc::~Bloc(){
	    if (nullptr != texture) SDL_DestroyTexture(texture);
	std::cout << "Bloc.texture free" << std::endl;

}