#include "config.h"

	Config::Config(int sw, int sh, int bs){
		SCREEN_WIDTH = sw;
		SCREEN_HEIGHT = sh;
		BLOCK_SIZE = bs;
		GRID_WIDTH = SCREEN_WIDTH/BLOCK_SIZE;
		GRID_HEIGHT = SCREEN_HEIGHT/BLOCK_SIZE;

    }
    int Config::getSCREEN_WIDTH() {
    	return SCREEN_WIDTH;
    }
    int Config::getSCREEN_HEIGHT() {
    	return SCREEN_HEIGHT;
    }
    int Config::getBLOCK_SIZE() {
    	return BLOCK_SIZE;
    }
    int Config::getGRID_WIDTH(){
    	return GRID_WIDTH;
    }
    int Config::getGRID_HEIGHT() {
    	return GRID_HEIGHT;
    }
    void Config::affiche(){

    	std::cout << "BLOCK_SIZE = " << BLOCK_SIZE << std::endl;
    	std::cout << "SCREEN_WIDTH = " << SCREEN_WIDTH << std::endl;
    	std::cout << "SCREEN_HEIGHT = " << SCREEN_HEIGHT << std::endl;
    	std::cout << "GRID_WIDTH = " << GRID_WIDTH << std::endl;
    	std::cout << "GRID_HEIGHT = " << GRID_HEIGHT << std::endl;
    }
  	
	 void Config::load(const std::string &filename) {
	    std::ifstream file(filename);

	    if (file.is_open()) {
	        file >> SCREEN_WIDTH;
	        file >> SCREEN_HEIGHT;
	        file >> BLOCK_SIZE;

	        // Mettez à jour les attributs GRID_WIDTH et GRID_HEIGHT en fonction des nouvelles valeurs.
	        GRID_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
	        GRID_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;

	        file.close();
	    } else {
	        std::cerr << "Unable to open file " << filename << std::endl;
	    }
	}