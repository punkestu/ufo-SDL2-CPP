#include <SDL.h>
#include <iostream>
#include <vector>

class ufo{
private:
    SDL_Rect body[2];
    SDL_Rect lightW;
    SDL_Rect* ground;
    int light;
    SDL_Rect wind[2];
    bool movD[4];
    bool beam;
public:
    ufo(SDL_Rect* ground){
        this->ground = ground;
        body[0] = {220,240,40,10};
        body[1] = {body[0].x+10, body[0].y-10, 20,10};
        light = 0;
        beam = 0;
        lightW = {body[0].x,body[0].y+body[0].h,body[0].w,light};
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
            if(event->key.keysym.sym == SDLK_SPACE){
                beam = 1;
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
            if(event->key.keysym.sym == SDLK_SPACE){
                beam = 0;
            }
        }
    }

    void movX(int dir, int spd){
        body[0].x+=dir*spd;
        //body[1] = {body[0].x+10, body[0].y-10, 20,10};
        lightW = {body[0].x,body[0].y+body[0].h,body[0].w,light};
    }
    void movY(int dir, int spd){
        body[0].y+=dir*spd;
        //body[1] = {body[0].x+10, body[0].y-10, 20,10};
        lightW = {body[0].x,body[0].y+body[0].h,body[0].w,light};
    }

    void update(){
        wind[0] = {0,0,0,0};
        wind[1] = {0,0,0,0};
        body[1] = {body[0].x+10, body[0].y-10, 20,10};
        if(movD[0]){
            body[1].x=body[0].x+5;
            movX(-1, 1);
            wind[0] = {body[1].x + body[1].w + 5, body[1].y, 10, 5};
            wind[1] = {body[0].x + body[0].w + 5, body[0].y, 10, 5};
        }
        if(movD[1]){
            body[1].x=body[0].x+15;
            movX(1,1);
            wind[0] = {body[1].x - 15, body[1].y, 10, 5};
            wind[1] = {body[0].x - 15, body[0].y, 10, 5};
        }
        if(movD[2]){
            movY(-1,1);
        }
        if(movD[3]){
            body[1].h = 15;
            movY(1,1);
        }
        if(beam){
            light = 480-(body[0].y+body[0].h)+20;
        }else{
            light = 0;
        }
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRects(renderer, body, 2);
        SDL_RenderFillRects(renderer, wind, 2);
        SDL_SetRenderDrawColor(renderer, 255,255,0,255);
        SDL_RenderFillRect(renderer, &lightW);
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
    SDL_Rect* getBody(){return &body[0];}
    bool getBeam(){return beam;}
};

class cow{
private:
    SDL_Rect body[4];
public:
    cow(int pos){
        body[0] = {pos+20,480-15,20,10};
        body[1] = {pos+20,480-5,5,5};
        body[2] = {pos+35,480-5,5,5};
        body[3] = {pos+35,480-25,12,12};
    }
    void movX(short dir, int spd){
        body[0].x+=dir*spd;
        body[1].x+=dir*spd;
        body[2].x+=dir*spd;
        body[3].x+=dir*spd;
    }
    void movY(short dir, int spd){
        body[0].y+=dir*spd;
        body[1].y+=dir*spd;
        body[2].y+=dir*spd;
        body[3].y+=dir*spd;
    }
    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRects(renderer, body, 4);
    }
    SDL_Rect getBody(){return {body[0].x,body[3].y,27,25};}
};

class camera{
private:
    ufo* mufo;
    std::vector<cow*>* cows;
    //cow* mcow;
    SDL_Rect* ground;
public:
    camera(ufo* mufo, std::vector<cow*>* cows, SDL_Rect* ground){
        this->mufo = mufo;
        this->cows = cows;
        this->ground = ground;
    }

    void movX(short dir, int spd){
        mufo->movX(-dir, spd);
        for(unsigned int i = 0; i < cows->size(); i++){
            (*cows)[i]->movX(-dir, spd);
        }
        //ground->x+=(-dir)*spd;
    }
    void movY(short dir, int spd){
        mufo->movY(-dir, spd);
        ground->y+=(-dir)*spd;
        for(unsigned int i = 0; i < cows->size(); i++){
            (*cows)[i]->movY(-dir, spd);
        }
    }
    void update(){
        movX(mufo->getMovX(),1);
        movY(mufo->getMovY(),1);
    }
};

int main(int argc, char* argv[])
{
    SDL_Window* window = SDL_CreateWindow("apps", 100,100,480,480,false);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect ground = {0,480,480,40};
    ufo mufo = ufo(&ground);
    std::vector<cow*> cows;
    cows.push_back(new cow(20));
    cows.push_back(new cow(40));
    cows.push_back(new cow(180));

    camera mcamera = camera(&mufo,&cows,&ground);

    SDL_Event event;
    int frame = SDL_GetTicks();
    bool dead = false;
    while(true){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        if(!dead){
            mufo.render(renderer);
        }
//        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
//        SDL_Rect temp = cow1.getBody();
//        SDL_RenderFillRect(renderer, &temp);
        for(unsigned int i = 0; i < cows.size(); i++){
            cows[i]->render(renderer);
        }
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &ground);

        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
            break;
        }
        mufo.control(&event);

        if(!dead){
            if(SDL_GetTicks()-frame >= 1000/60){
                frame = SDL_GetTicks();
                mcamera.update();
                mufo.update();
                ground.h=480-ground.y;
                for(std::vector<cow*>::iterator it = cows.begin(); it != cows.end(); it++){
                    if((*it)->getBody().x<mufo.getBody()->x+mufo.getBody()->w && (*it)->getBody().x+(*it)->getBody().w>mufo.getBody()->x &&
                       (*it)->getBody().y<mufo.getBody()->y+mufo.getBody()->h && (*it)->getBody().y+(*it)->getBody().h>mufo.getBody()->y){
                            cows.erase(it);
                            it--;
                            continue;
                       }
                    if(mufo.getBeam()){
                        if((*it)->getBody().x<mufo.getBody()->x+mufo.getBody()->w && (*it)->getBody().x+(*it)->getBody().w>mufo.getBody()->x){
                            (*it)->movY(-1,1);
                        }else{
                            if((*it)->getBody().y+(*it)->getBody().h<ground.y){
                                (*it)->movY(1,1);
                            }
                        }
                    }else{
                        if((*it)->getBody().y+(*it)->getBody().h<ground.y){
                            (*it)->movY(1,1);
                        }
                    }
                }
            }

        }else{
            if(event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN2)){
                break;
            }
        }

        if(mufo.getBody()->y+mufo.getBody()->h > ground.y){
            dead = true;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
