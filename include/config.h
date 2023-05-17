/*

Cette class permet de s'affranchir des variablles globales tels que la taille de la fenêtre, des blocs etc..
On rentre c'est valeur via un fichier de configuration config.txt ou bien on les rendres directement avec le constructeur de la class.
Pour ensuite récupérer ces valeurs, on utilise les méthodes de la class get...();
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string.h>
#include <fstream>

class Config {
private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int BLOCK_SIZE;
    int GRID_WIDTH;
    int GRID_HEIGHT;
public:
    Config(int sw = 640, int sh = 480, int bs = 32);
    int getSCREEN_WIDTH();
    int getSCREEN_HEIGHT();
    int getBLOCK_SIZE();
    int getGRID_WIDTH();
    int getGRID_HEIGHT();
    void affiche();
    void load(const std::string &filename = "config.txt");
};

#endif