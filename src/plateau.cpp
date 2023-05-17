#include "plateau.h"
#include "SDL_fonction.h"


int Plateau::nb_plat = 0; 

Plateau::Plateau(int n, int bs, int sw, int sh,int gw, int gh) {
	num = n;
	BLOC_SIZE = bs; 
	SCREEN_WIDTH = sw;
	SCREEN_HEIGHT = sh;
	GRID_WIDTH = gw;
	GRID_HEIGHT = gh;

	tab = new int[GRID_WIDTH*GRID_HEIGHT];
	for (int i = 0; i < GRID_WIDTH*GRID_HEIGHT; ++i) tab[i] = 0;
	nb_plat ++;

}

Plateau::~Plateau() {
	delete[] tab;
	nb_plat--;
}

void Plateau::map(const std::string &nomFichier) {
    std::ifstream fichier(nomFichier);

    if (!fichier.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier." << std::endl;
    }
    else {
	    int k = 0;
	    std::string ligne;
	    while (std::getline(fichier, ligne)) {
	        for (char caractere : ligne) {
	            // std::cout << caractere;
	            // if (k%20 == 0) std::cout << std::endl;

	            switch(caractere){
				case '0':
					//futur fonctionalité 
					break;
				case '1':
					tab[k] = 1;
					break;
				case '2':
					//futur fonctionalité 
					break;

				default:
					break;
				}

				k++;
	        }
	    }
	    std::cout << std::endl;
	    fichier.close();
	}
}

void Plateau::edit(int k, int value){
	tab[k] = value;
}

void Plateau::edit(int i, int j, int value){
	int k = i + j * GRID_WIDTH;
	tab[k] = value;
}

void Plateau::save(){

	std::string filename;
    std::cout << "Veuillez entrer le nom du fichier ou sauvegarder la map: ";
    std::cin >> filename;
    filename = "./asset/map_save/" + filename + ".txt";

    std::ofstream fichier(filename);
    int compt = 0;
    if (fichier.is_open()) {
        for (int k = 0; k<(GRID_WIDTH * GRID_HEIGHT); k++) {
            fichier << tab[k];
            compt++;
            if (compt == GRID_WIDTH) {
            	compt = 0;
            	fichier << std::endl;
            }
        }
        fichier.close();
        std::cout << "save" << std::endl;
    } else {
        std::cerr << "Erreur : impossible de créer le fichier." << std::endl;
    }

}

void Plateau::draw(SDL_Renderer *renderer, SDL_Texture *texture, const Bloc &mur) {
    SDL_Rect rect;
    rect.w = BLOC_SIZE;
    rect.h = BLOC_SIZE;
    for(int k = 0; k<GRID_WIDTH*GRID_HEIGHT; k++){
        if (tab[k] == 1) {
        	rect.x = (k % GRID_WIDTH) * BLOC_SIZE;
        	rect.y = (k / GRID_WIDTH) * BLOC_SIZE;
            drawImage(mur.texture, renderer, texture, nullptr, &rect);
        }
    }

}

void Plateau::affiche() {
	int compt = 0;
	for(int k = 0; k < GRID_WIDTH * GRID_HEIGHT; k++){
		std::cout << tab[k];
		compt++;
		if (compt == GRID_WIDTH){
			compt = 0;
			std::cout << std::endl;
		}
	}
}

void Plateau::affiche(int i, int j){
	int k = i + GRID_WIDTH * j;
	std::cout << tab[k] << std::endl;
}

void Plateau::affiche(int k){
	std::cout << tab[k] << std::endl;
}

int Plateau::get(int k) {
	return tab[k];
}