#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "world.h"
//#include "goomba.h"

class game {  
public:

    void init() {
        gameIsRunning = true;
        renderer = new SDL_Renderer();
        renderer->initialise();
                    
        Player = new player();
        Player->loadTexture();
        int* whichIdsPresentInWorld;
        World = new world();
        assets = new tileAssetManager();
        whichIdsPresentInWorld = World->loadTileMap(WorldEnum::WORLD_1, LevelEnum::LEVEL_1); //I don't hate myself enough to continue generalising this code
        assets->initialise("Assets/Worlds/World1/spriteSheet.png", whichIdsPresentInWorld, World->getTotalNrOfIds());
        Camera = new camera();    
    }
     
    double getCurrentTimeSeconds() {
        timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec + tv.tv_usec / 1000000.0;
    }
       
    void play() {
        renderer->displayStartScreen();
        double lastTime = getCurrentTimeSeconds();
        renderer->turn_on_leds(0b00000011);
        while (gameIsRunning) {
            double currentTime = getCurrentTimeSeconds();
            deltaTime = (currentTime - lastTime) / 1000.0f; 
            if (deltaTime < 0.0001f) deltaTime = 0.0001f; // Prevent zero/negative delta time
            if (deltaTime > 0.05f) deltaTime = 0.05f;
            lastTime = currentTime;

            Player->resolveMovement(World, assets, deltaTime);    
            Camera->update(Player->getX(), Player->getY(), Player->getWidth(), Player->getHeight(), World->getPixelWidth(), World->getPixelHeigth(), deltaTime);
            render();  
        }
        clean();
    }

    void render() {
        renderer->clear(); //sets to sky btw
        
        Player->render(renderer, std::floor(Camera->getX()), std::floor(Camera->getY()));
        World->render(renderer, Camera, assets);
                     
        renderer->render();
    }
               
    void clean() {
        Player->destroy();
        delete Player;
        World->cleanup();
        assets->cleanup();
        delete World;
        delete assets;
        delete Camera;
        renderer->cleanup();
        delete renderer;
        printf("\n"); //'cause I compile the game in console and use Arch
    }

    bool running() const { return gameIsRunning; }

private:
    SDL_Renderer* renderer = nullptr;
    bool gameIsRunning = false;
    player* Player = nullptr;
    world* World = nullptr;
    camera* Camera = nullptr;
    tileAssetManager* assets = nullptr; 
    double deltaTime;
};

#endif

