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
        moveX = moveY = 0;
    }

    void control(SDL_Event* event){
        if(event->type = SDL_KEYDOWN){
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
        if(event->type = SDL_KEYUP){
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

    void update(){

    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRects(renderer, body, 2);
    }
};

int main(int argc, char* argv[])
{
    SDL_Window* window = SDL_CreateWindow("apps", 100,100,480,480,false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    ufo mufo = ufo();

    SDL_Event event;
    while(true){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        mufo.render(renderer);

        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
            break;
        }


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
