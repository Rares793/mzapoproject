#ifndef GOOMBA_H
#define GOOMBA_H
//This doesn't work on PC so no way I am gonna bother porting it
#include "collisionBox.h"


class goomba : public entity{
public:
    void loadTexture(SDL_Renderer* renderer) override{
        //printf("\nEntered loadTexture");    
        SDL_Surface* loadedSurface = IMG_Load("Assets/goomba16.png");
        if (!loadedSurface) {
        std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
        return;
    }
        entityTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (!entityTexture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return;
    }
        SDL_QueryTexture(entityTexture, NULL, NULL, &entityRect.w, &entityRect.h);
        //printf("\nLoaded Texture");         
        entityRect.x = static_cast<int>(entityX);
        entityRect.y = static_cast<int>(entityY);
        entityWidth = static_cast<float>(entityRect.w); 
        entityHeight = static_cast<float>(entityRect.h); 
        SDL_FreeSurface(loadedSurface);
        
        //printf("\nAbout to create CollisionBox");
        CollisionBox = new collisionBox();
        //printf("\nHappy");
    }       
    
    void activationCheck(float playerX, float playerY){ // playerY is unused
    // This condition seems a bit too broad. It will activate if the player is anywhere
    // near one screen width away on either side.
    if(playerX + SCREEN_WIDTH + activationWindow > entityX || entityX + SCREEN_WIDTH + activationWindow > playerX){
        active = true; // Use true/false
        return;
    }
    active = false; // Use true/false
}                     
                                
    void resolveMovement(world* gameWorld, tileAssetManager* assets, float deltaTime) override{
        if (!active) {
        entityVX = 0; // Don't accumulate velocity if not active
        return;
        }
        printf("\nEntered resolveMovement");
        if(movingLeft)
            entityVX = -GOOMBA_MOVE_SPEED;
        else entityVX = GOOMBA_MOVE_SPEED;
                 
        if(!onGround) {
            entityVY += GRAVITY * deltaTime;
            if(entityVY > MAX_FALL_SPEED) entityVY = MAX_FALL_SPEED;
        }
        entityY += entityVY * deltaTime;
        onGround = false;
        CollisionBox->checkCollisionY(gameWorld, assets, entityX, entityY, entityVX, entityVY, entityRect, onGround);                                                   
        entityX += entityVX * deltaTime;
        int hitWhatDirection = CollisionBox->checkCollisionX(gameWorld, assets, entityX, entityY, entityVX, entityVY, entityRect, onGround, entityHeight, entityWidth);
        printf("\nGot past CollisionX and Y");
        if(hitWhatDirection == 1)
            movingLeft = 0;
        else{
            printf("\nDirection: %d, EntityY: %d", movingLeft, entityY);
            //hitWhatDirection = CollisionBox->checkEdgeRight(gameWorld, assets, entityX, entityY, entityVX, entityRect, entityHeight, entityWidth);
            //hitWhatDirection = CollisionBox->checkEdgeLeft(gameWorld, assets, entityX, entityY, entityVX, entityRect , entityHeight, entityWidth);
        }                                  
                                                                                                  printf("\nGot past the if");                                                     
        CollisionBox->checkCollisionWithWorld(gameWorld->getPixelWidth(), gameWorld->getPixelHeigth(), entityX, entityY, entityVX, entityVY, entityRect, onGround, alive);
        printf("\nGot past world collision");
        entityRect.x = static_cast<int>(entityX);
        entityRect.y = static_cast<int>(entityY);
    }
                                  
    void initialisePosition(goombaSpawns spawn){
        entityX = static_cast<float>(spawn.x) * TILE_SIZE; // X world pos based on tile Column
        entityY = static_cast<float>(spawn.y) * TILE_SIZE; // Y world pos based on tile Row
                                                               // Adjust Y to place feet on tile if needed:
                                                               // entityY = static_cast<float>(spawn_data.tileRow + 1) * TILE_SIZE - entityHeight;
    alive = true; // Use true/false for bools
    movingLeft = true; // Initialize movement direction
    active = false; // Start inactive
    onGround = false; // Will be determined by first collision check
    // Initialize entityRect based on new entityX, entityY
    entityRect.x = static_cast<int>(entityX);
    entityRect.y = static_cast<int>(entityY);
    } 
                                           
    void render(SDL_Renderer* renderer, int cameraX, int cameraY) override{
        SDL_Rect screenRect = {entityRect.x - cameraX, entityRect.y - cameraY, entityRect.w, entityRect.h};
        SDL_RenderCopy(renderer, entityTexture, NULL, &screenRect);
    }                              
                                                  
    void destroy() override{
        if (entityTexture) {
            SDL_DestroyTexture(entityTexture);
            entityTexture = nullptr;
        }
        delete CollisionBox;
        CollisionBox = nullptr;
    }      
     void handleInput() override{
         printf("\nThis is useless, it's here so the compiler doesn't kill me\n");
     }                           
    bool isActive(){
        return active;
    }  
                                
    bool isAlive(){
        return alive;
    }                                
private:    
    collisionBox* CollisionBox = nullptr; 
    bool alive;
    bool movingLeft;
    bool active = 0;     
    float activationWindow = 40.0f; //when the goomba is inside the screen dimension +- activationWindnow activation window (sorry for the name)
                                    // then the goomba will start getting active
    //goombaSpawns spawns[11];                      
};
#endif
