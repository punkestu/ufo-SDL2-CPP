#include <SDL.h>
#include <iostream>
#include <vector>

class ufo{
private:
    SDL_Rect body[2];
    SDL_Rect lightW;
    bool movD[4];
public:
    ufo(){
        body[0] = {220,240,40,10};
        body[1] = {body[0].x+10, body[0].y-10, 20,10};
        movD[0] = movD[1] = movD[2] = movD[3] = 0;
    }

    void control(SDL_Event* event){
        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_a){
                movD[0] = 1;
            }
            if(event->key.keysym.sym == SDLK_d){
                movD[1] = 1;
            }
            if(event->key.keysym.sym == SDLK_w){
                movD[2] = 1;
            }
            if(event->key.keysym.sym == SDLK_s){
                movD[3] = 1;
            }
        }
        if(event->type == SDL_KEYUP){
            if(event->key.keysym.sym == SDLK_a){
                movD[0] = 0;
            }
            if(event->key.keysym.sym == SDLK_d){
                movD[1] = 0;
            }
            if(event->key.keysym.sym == SDLK_w){
                movD[2] = 0;
            }
            if(event->key.keysym.sym == SDLK_s){
                movD[3] = 0;
            }
        }
    }

    void movX(int dir, int spd){
        body[0].x+=dir*spd;
        body[1] = {body[0].x+10, body[0].y-10, 20,10};
    }
    void movY(int dir, int spd){
        body[0].y+=dir*spd;
        body[1] = {body[0].x+10, body[0].y-10, 20,10};
    }

    void update(){
        if(movD[0]){
            movX(-1, 2);
        }
        if(movD[1]){
            movX(1,2);
        }
        if(movD[2]){
            movY(-1,2);
        }
        if(movD[3]){
            movY(1,2);
        }
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRects(renderer, body, 2);
    }
    short getMovX(){
        if(movD[0]&&!movD[1]){return -1;}
        if(!movD[0]&&movD[1]){return 1;}
        return 0;
    }
    short getMovY(){
        if(movD[2]&&!movD[3]){return -1;}
        if(!movD[2]&&movD[3]){return 1;}
        return 0;
    }
};

class cow{

};

class camera{
private:
    ufo* mufo;
    //std::vector<cow*>* cows;
    SDL_Rect* ground;
public:
    camera(ufo* mufo, SDL_Rect* ground){
        this->mufo = mufo;
        //this->cows = cows;
        this->ground = ground;
    }

    void movX(short dir, int spd){
        mufo->movX(-dir, spd);
        ground->x+=(-dir)*spd;
    }
    void movY(short dir, int spd){
        mufo->movY(-dir, spd);
        ground->y+=(-dir)*spd;
    }

    void update(){
        movX(mufo->getMovX(),2);
        movY(mufo->getMovY(),2);
    }
};

int main(int argc, char* argv[])
{
    SDL_Window* window = SDL_CreateWindow("apps", 100,100,480,480,false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    ufo mufo = ufo();
    SDL_Rect ground = {0,440,480,40};

    camera mcamera = camera(&mufo, &ground);

    SDL_Event event;
    int frame = SDL_GetTicks();
    while(true){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        mufo.render(renderer);
        SDL_RenderFillRect(renderer, &ground);

        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
            break;
        }
        mufo.control(&event);

        if(SDL_GetTicks()-frame >= 1000/60){
            frame = SDL_GetTicks();
            mufo.update();
            mcamera.update();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
