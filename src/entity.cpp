#include "entity.h"
#include "SDL_fonction.h"



int Entity::nb_entity = 0;

Entity::Entity(int x, int y, int ENTITY_SIZE, float moove_speed_) {
    rect.x = x;
    rect.y = y;
    rect.w = ENTITY_SIZE;
    rect.h = ENTITY_SIZE;
    moove_speed = moove_speed_;
    nb_entity ++;
}

Entity::~Entity() {
    if (nullptr != texture) SDL_DestroyTexture(texture);
    nb_entity --;
    std::cout << "Entity.texture free" << std::endl;
}

void Entity::load(const char * path, SDL_Renderer *renderer) {
    texture = loadImage(path, renderer);
}


void Entity::affiche() {
    std::cout << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;
}

void Entity::deplace(int dx, int dy,const Plateau &plat) {
    int SCREEN_WIDTH = 640; //provisoire
    int SCREEN_HEIGHT = 480;
    int BLOCK_SIZE = rect.w;
    if (nullptr != plat.tab) {
        SDL_Rect mur_rect;

        mur_rect.w = BLOCK_SIZE;
        mur_rect.h = BLOCK_SIZE;
        int nb_lignes = SCREEN_HEIGHT/BLOCK_SIZE;
        int nb_colonnes = SCREEN_WIDTH/BLOCK_SIZE;
        std::cout << "!!" <<nb_colonnes<<std::endl;

        bool intersection = 0;

        rect.x += dx*rect.w;
        rect.y += dy*rect.h;

        for(int k = 0; k < (SCREEN_WIDTH / BLOCK_SIZE) * (SCREEN_HEIGHT / BLOCK_SIZE); k++) {
            if (plat.tab[k] == 1){
                int j = k / nb_colonnes;
                int i = k % nb_colonnes;
                mur_rect.x = i * BLOCK_SIZE;
                mur_rect.y = j * BLOCK_SIZE;
                if (SDL_HasIntersection(&mur_rect, &rect)){
                    intersection = 1;
                }
            }

        }
        if (intersection == 1){
            rect.x -= dx*rect.w;
            rect.y -= dy*rect.h;
        }
        //si 1 alors on fais rien
    }
    else {
        rect.x += dx*rect.w;
        rect.y += dy*rect.h;
    }

    // La suite permet de refermet le bord gauche sur droit et le bord haut sur le bas

    if (rect.x < 0) {
        rect.x = SCREEN_WIDTH - BLOCK_SIZE;
    } else if (rect.x > SCREEN_WIDTH - BLOCK_SIZE) {
        rect.x = 0;
    }

    if (rect.y < 0) {
        rect.y = SCREEN_HEIGHT - BLOCK_SIZE;
    } else if (rect.y > SCREEN_HEIGHT - BLOCK_SIZE) {
        rect.y = 0;
    }


}

void Entity::moove(int dx, int dy, Plateau &plat){
    
    float deltaTime = 1/30.0; // Ici on considère que l‡e jeu tourne à 30 fps fixe   // NOTE : cette pratique est dangeureuse !!
    int SCREEN_WIDTH = 640; //provisoire
    int SCREEN_HEIGHT = 480;
    int BLOCK_SIZE = rect.w;

    if (nullptr != plat.tab) {
        SDL_Rect mur_rect;
        mur_rect.w = BLOCK_SIZE;
        mur_rect.h = BLOCK_SIZE;

        int nb_lignes = SCREEN_HEIGHT/BLOCK_SIZE;
        int nb_colonnes = SCREEN_WIDTH/BLOCK_SIZE;

        bool intersection = 0; // si intersection = 1 alors il y à intersectoin !

        // on moove le player

        rect.x += moove_speed*deltaTime*dx;
        rect.y += moove_speed*deltaTime*dy;
        
        int w = rect.w;
        int h = rect.h;

        // Test de collision

        SDL_Point P0, P1, P2, P3;

        P0.x = rect.x;          
        P0.y = rect.y;

        P1.x = rect.x + w;          
        P1.y = rect.y;

        P2.x = rect.x + w;          
        P2.y = rect.y + h;

        P3.x = rect.x;          
        P3.y = rect.y + h;

        int indice0, indice1, indice2, indice3;

        indice0 = getCaseIndex(P0);
        indice1 = getCaseIndex(P1);
        indice2 = getCaseIndex(P2);
        indice3 = getCaseIndex(P3);


        if (plat.get(indice0) == 1) {
            intersection = 1;
        } else if (plat.get(indice1) == 1) {
            intersection = 1;
        } else if (plat.get(indice2) == 1) {
            intersection = 1;
        } else if (plat.get(indice3) == 1) {
            intersection = 1;
        }

        if (intersection == 1){
            rect.x -= moove_speed*deltaTime*dx;
            rect.y -= moove_speed*deltaTime*dy;
        }
        //si 1 alors on fais rien
    }
    else {
        // Si jamais on fourni à la fonction moove un tableau nullptr alors on ne check pas les collisions
        rect.x += moove_speed*deltaTime*dx;
        rect.y += moove_speed*deltaTime*dy;
    }

    // La suite permet de refermet le bord gauche sur droit et le bord haut sur le bas

    if (rect.x < 0) {
        rect.x = SCREEN_WIDTH - BLOCK_SIZE;
    } else if (rect.x > SCREEN_WIDTH - BLOCK_SIZE) {
        rect.x = 0;
    }

    if (rect.y < 0) {
        rect.y = SCREEN_HEIGHT - BLOCK_SIZE;
    } else if (rect.y > SCREEN_HEIGHT - BLOCK_SIZE) {
        rect.y = 0;
    }

    afficheRect(rect);
}


// Version de moove que l'on utilisera pour la classe fille Player


void Entity::moove(Direction direction, Plateau &plat, Config &config){
    
    float deltaTime = 1/30.0; // Ici on considère que l‡e jeu tourne à 30 fps fixe   // NOTE : cette pratique est dangeureuse !!
    int SCREEN_WIDTH = config.getSCREEN_WIDTH();
    int SCREEN_HEIGHT = config.getSCREEN_HEIGHT();
    int BLOCK_SIZE = rect.w;

    if (nullptr != plat.tab) {
        SDL_Rect mur_rect;
        mur_rect.w = BLOCK_SIZE;
        mur_rect.h = BLOCK_SIZE;

        int nb_lignes = SCREEN_HEIGHT/BLOCK_SIZE;
        int nb_colonnes = SCREEN_WIDTH/BLOCK_SIZE;

        bool intersection = 0; // si intersection = 1 alors il y à intersectoin !

        SDL_Rect rect_tmp = rect;

        // on moove un player virtuelle
        switch (direction)
        {
        case RIGHT:
            rect_tmp.x += moove_speed*deltaTime;
            break;

        case LEFT:
            rect_tmp.x -= moove_speed*deltaTime;
            break;

        case  UP:
            rect_tmp.y -=  moove_speed*deltaTime; 
            break;

        case DOWN:
            rect_tmp.y += moove_speed*deltaTime; 
        default:

            break;
        }

   
        int w = rect_tmp.w;
        int h = rect_tmp.h;

        // Test de collision

        SDL_Point P0, P1, P2, P3;

        P0.x = rect_tmp.x;          
        P0.y = rect_tmp.y;

        P1.x = rect_tmp.x + w;          
        P1.y = rect_tmp.y;

        P2.x = rect_tmp.x + w;          
        P2.y = rect_tmp.y + h;

        P3.x = rect_tmp.x;          
        P3.y = rect_tmp.y + h;

        int indice0, indice1, indice2, indice3;
        if (rect.w == 32) {
            indice0 = getCaseIndex(P0);
            indice1 = getCaseIndex(P1);
            indice2 = getCaseIndex(P2);
            indice3 = getCaseIndex(P3);
        }
        else if (rect.w == 64) {
            indice0 = getCaseIndex64(P0);
            indice1 = getCaseIndex64(P1);
            indice2 = getCaseIndex64(P2);
            indice3 = getCaseIndex64(P3);
        }
        else printf("error size when mooving\n");
        

        if (plat.get(indice0) == 1) {
            intersection = 1;
        } else if (plat.get(indice1) == 1) {
            intersection = 1;
        } else if (plat.get(indice2) == 1) {
            intersection = 1;
        } else if (plat.get(indice3) == 1) {
            intersection = 1;
        }

        if (!intersection){
            rect = rect_tmp;
        }
        //si 1 alors on fais rien
    }
    else {

        switch (direction)
        {
        case RIGHT:
            rect.x += moove_speed*deltaTime;
            break;

        case LEFT:
            rect.x -= moove_speed*deltaTime;
            break;

        case  UP:
            rect.y -=  moove_speed*deltaTime; 
            break;

        case DOWN:
            rect.y += moove_speed*deltaTime; 
        default:

            break;
        }

    }

    // La suite permet de refermet le bord gauche sur droit et le bord haut sur le bas

    if (rect.x < 0) {
        rect.x = SCREEN_WIDTH - BLOCK_SIZE;
    } else if (rect.x > SCREEN_WIDTH - BLOCK_SIZE) {
        rect.x = 0;
    }
    
    if (rect.y < 0) {
        rect.y = SCREEN_HEIGHT - BLOCK_SIZE;
    } else if (rect.y > SCREEN_HEIGHT - BLOCK_SIZE) {
        rect.y = 0;
    }

}


int Entity::draw(SDL_Renderer* renderer, SDL_Texture *textureCible) {
    if (nullptr != texture) {
        SDL_Rect srcRect = {0, 0, rect.w, rect.h};
        //int drawImage(SDL_Texture *image_texture, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *rectCut, SDL_Rect *rectPos, int angle = 0, const SDL_RendererFlip flip = SDL_FLIP_NONE);
        drawImage(texture, renderer, textureCible, &srcRect, &rect);
    }
    else{
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (SDL_RenderFillRect(renderer, &rect))
        {
            std::cout << "Erreur FillREct\n";
            return 1;
        }
    }

 
    return 0;
}


// fonction d'acces par copie 

SDL_Rect Entity::getRect() {
    return rect;
}

float Entity::getSpeed() {
    return moove_speed;
}

int Entity::getNumberEntity() {
    return nb_entity;
}

// Fonction de modification direct (principalement pour debug)